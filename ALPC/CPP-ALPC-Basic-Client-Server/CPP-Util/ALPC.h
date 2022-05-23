#pragma once

#include <Windows.h>
#include <winternl.h>

////
//
// CONSTANTS
//
////

// PORT ATTRIBUTE FLAGS
#define ALPC_PORTFLG_NONE 0x0
#define ALPC_PORTFLG_LPCPORT 0x1000 // Only usable in kernel
#define ALPC_PORTFLG_ALLOWIMPERSONATION 0x10000 // Can be set by client to allow server to impersonation this client
#define ALPC_PORTFLG_ALLOW_LPC_REQUESTS 0x20000
#define ALPC_PORTFLG_WAITABLE_PORT 0x40000	// Allow port to be used with synchronization mechanisms like Semaphores
#define ALPC_PORTFLG_SYSTEM_PROCESS 0x100000 // Only usable in kernel
#define ALPC_PORTFLG_ALLOW_DUP_OBJECT 0x80000
#define ALPC_PORTFLG_LRPC_WAKE_POLICY1 0x200000
#define ALPC_PORTFLG_LRPC_WAKE_POLICY2 0x400000
#define ALPC_PORTFLG_LRPC_WAKE_POLICY3 0x800000
#define ALPC_PORTFLG_DIRECT_MESSAGE 0x1000000	// There are 5 queues, Main queue, Direct message queue, Large message queue, Pending queue, Canceled queue... guess this attribute specifies to use the direct message queue instead of main queue

// Uncertain
//#define ALPC_PORTFLG_ACCEPT_INDIRECT_HANDLES (from https://github.com/microsoft/terminal/blob/059f2031583fd22fb7b5f498e4a30196cf3150d4/src/interactivity/onecore/ConIoSrvComm.cpp)
//#define ALPC_PORTFLG_ALLOW_MULTI_HANDLE_ATTRIBUTE 0x2000000


// ERROS
/// just the ones i need to integrate into my code flow
const NTSTATUS STATUS_BUFFER_TOO_SMALL = 0xC0000023;
const NTSTATUS STATUS_PORT_CONNECTION_REFUSED = 0xC0000041;
const NTSTATUS STATUS_ACCESS_DENIED = 0xC0000022;
const NTSTATUS STATUS_SERVER_SID_MISMATCH = 0xC00002A0;

// ALPC Message Attributes
#define ALPC_MESSAGE_SECURITY_ATTRIBUTE 0x80000000
#define ALPC_MESSAGE_VIEW_ATTRIBUTE 0x40000000
#define ALPC_MESSAGE_CONTEXT_ATTRIBUTE 0x20000000
#define ALPC_MESSAGE_HANDLE_ATTRIBUTE 0x10000000
#define ALPC_MESSAGE_TOKEN_ATTRIBUTE 0x8000000
#define ALPC_MESSAGE_DIRECT_ATTRIBUTE 0x4000000
#define ALPC_MESSAGE_WORK_ON_BEHALF_ATTRIBUTE 0x2000000
#define ALPC_MESSAGE_ATTRIBUTE_ALL ALPC_MESSAGE_SECURITY_ATTRIBUTE | ALPC_MESSAGE_VIEW_ATTRIBUTE | ALPC_MESSAGE_CONTEXT_ATTRIBUTE | ALPC_MESSAGE_HANDLE_ATTRIBUTE | ALPC_MESSAGE_TOKEN_ATTRIBUTE | ALPC_MESSAGE_DIRECT_ATTRIBUTE | ALPC_MESSAGE_WORK_ON_BEHALF_ATTRIBUTE

// ALPC Message Flags
#define ALPC_MSGFLG_NONE 0x0
#define ALPC_MSGFLG_REPLY_MESSAGE 0x1
#define ALPC_MSGFLG_LPC_MODE 0x2
#define ALPC_MSGFLG_RELEASE_MESSAGE 0x10000
#define ALPC_MSGFLG_SYNC_REQUEST 0x20000		// synchronous message, needs a receive buffer or else error 0xC0000705 is returned
#define ALPC_MSGFLG_WAIT_USER_MODE 0x100000
#define ALPC_MSGFLG_WAIT_ALERTABLE 0x200000
#define ALPC_MSGFLG_WOW64_CALL 0x80000000

// ALPC Connection FLAGS
/// From: https://recon.cx/2008/a/thomas_garnier/LPC-ALPC-paper.pdf
#define ALPC_SYNC_CONNECTION 0x20000 // Synchronous connection request
#define ALPC_USER_WAIT_MODE 0x100000 // Wait in user mode
#define ALPC_WAIT_IS_ALERTABLE 0x200000 // Wait in alertable mode

// 0xc0020000 connection flag causing SF flag to be "1" in nt!AlpcpExposeViewAttribute and therefore calling nt!AlpcpExposeViewAttribute

////
//
// STRUCTS
//
////

// Based on https://github.com/googleprojectzero/sandbox-attacksurface-analysis-tools/blob/main/NtApiDotNet/NtAlpcNative.cs#L82-L97
enum ALPC_MESSAGE_TYPE
{
	None = 0,
	Request = 1,
	Reply = 2,
	Datagram = 3,
	LostReply = 4,
	PortClosed = 5,
	ClientDied = 6,
	Exception = 7,
	DebugEvent = 8,
	ErrorEvent = 9,
	ConnectionRequest = 10,
	ConnectionReply = 11,
	UNKNOWN12 = 12,	// Seems to be something like an empty reply, PID and TID pointing to own thread
	PortDisconnected = 13 // Used by the kernel when disconnecting an exception port.
};
// End Based  ON

typedef struct _ALPC_PORT_ATTRIBUTES
{
    unsigned long Flags;
    SECURITY_QUALITY_OF_SERVICE SecurityQos;
    unsigned __int64 MaxMessageLength;
    unsigned __int64 MemoryBandwidth;
    unsigned __int64 MaxPoolUsage;
    unsigned __int64 MaxSectionSize;
    unsigned __int64 MaxViewSize;
    unsigned __int64 MaxTotalSectionSize;
	ULONG DupObjectTypes;
#ifdef _WIN64
	ULONG Reserved;
#endif
} ALPC_PORT_ATTRIBUTES, * PALPC_PORT_ATTRIBUTES;

typedef struct _ALPC_MESSAGE_ATTRIBUTES
{
	ULONG AllocatedAttributes;
	ULONG ValidAttributes;
} ALPC_MESSAGE_ATTRIBUTES, * PALPC_MESSAGE_ATTRIBUTES;

typedef struct _PORT_MESSAGE
{
	union
	{
		struct
		{
			USHORT DataLength;
			USHORT TotalLength;
		} s1;
		ULONG Length;
	} u1;
	union
	{
		struct
		{
			USHORT Type;
			USHORT DataInfoOffset;
		} s2;
		ULONG ZeroInit;
	} u2;
	union
	{
		CLIENT_ID ClientId;
		double DoNotUseThisField;
	};
	ULONG MessageId;
	union
	{
		SIZE_T ClientViewSize; // only valid for LPC_CONNECTION_REQUEST messages
		ULONG CallbackId; // only valid for LPC_REQUEST messages
	};
} PORT_MESSAGE, * PPORT_MESSAGE;


// FROM https://docs.rs/ntapi/0.3.6/ntapi/ntlpcapi/struct.ALPC_HANDLE_ATTR.html
typedef struct _ALPC_DATA_VIEW_ATTR {
	ULONG Flags;
	HANDLE SectionHandle;
	PVOID ViewBase;
	SIZE_T ViewSize;
} ALPC_DATA_VIEW_ATTR, * PALPC_DATA_VIEW_ATTR;

typedef struct _ALPC_TOKEN_ATTR
{
	ULONGLONG TokenId;
	ULONGLONG AuthenticationId;
	ULONGLONG ModifiedId;
} ALPC_TOKEN_ATTR, * PALPC_TOKEN_ATTR;

typedef struct _AlpcPortContext {
	HANDLE Handle;
} AlpcPortContext, * PAlpcPortContext;

typedef struct _ALPC_CONTEXT_ATTR {
	PVOID PortContext;
	PVOID MessageContext;
	ULONG Sequence;
	ULONG MessageId;
	ULONG CallbackId;
} ALPC_CONTEXT_ATTR, * PALPC_CONTEXT_ATTR;

typedef struct _ALPC_SECURITY_ATTR {
	ULONG Flags;
	PSECURITY_QUALITY_OF_SERVICE pQOS;
	HANDLE ContextHandle;
} ALPC_SECURITY_ATTR, * PALPC_SECURITY_ATTR;


typedef struct _SECURITY_CLIENT_CONTEXT
{
	SECURITY_QUALITY_OF_SERVICE SecurityQos;	//_KALPC_SECURITY_DATA
	LPVOID ClientToken;
	UCHAR DirectlyAccessClientToken;
	UCHAR DirectAccessEffectiveOnly;
	UCHAR ServerIsRemote;
	_TOKEN_CONTROL ClientTokenControl;
} SECURITY_CLIENT_CONTEXT, * PSECURITY_CLIENT_CONTEXT;

// From https://github.com/hakril/PythonForWindows/blob/3149961634cda0029a288e6ffa7779e4492adb13/ctypes_generation/definitions/structures/alpc.txt
typedef struct _ALPC_DIRECT_ATTR
{
	HANDLE Event;
} ALPC_DIRECT_ATTR, * PALPC_DIRECT_ATTR;

typedef struct _ALPC_WORK_ON_BEHALF_ATTR
{
	ULONGLONG Ticket;
} ALPC_WORK_ON_BEHALF_ATTR, * PALPC_WORK_ON_BEHALF_ATTR;

typedef struct _ALPC_HANDLE_ATTR
{
	ULONG Flags;
	HANDLE Handle;
	ULONG ObjectType;
	ACCESS_MASK DesiredAccess;
} ALPC_HANDLE_ATTR, * PALPC_HANDLE_ATTR;

typedef struct _ALPC_HANDLE_ENTRY {
	LPVOID Object;
};

typedef struct _PORT_VIEW
{
	ULONG Length;
	HANDLE SectionHandle;
	ULONG SectionOffset;
	SIZE_T ViewSize;
	PVOID ViewBase;
	PVOID ViewRemoteBase;
} PORT_VIEW, * PPORT_VIEW;

typedef enum _ALPC_PORT_INFORMATION_CLASS
{
	AlpcBasicInformation, // q: out ALPC_BASIC_INFORMATION
	AlpcPortInformation, // s: in ALPC_PORT_ATTRIBUTES
	AlpcAssociateCompletionPortInformation, // s: in ALPC_PORT_ASSOCIATE_COMPLETION_PORT
	AlpcConnectedSIDInformation, // q: in SID
	AlpcServerInformation, // q: inout ALPC_SERVER_INFORMATION
	AlpcMessageZoneInformation, // s: in ALPC_PORT_MESSAGE_ZONE_INFORMATION
	AlpcRegisterCompletionListInformation, // s: in ALPC_PORT_COMPLETION_LIST_INFORMATION
	AlpcUnregisterCompletionListInformation, // s: VOID
	AlpcAdjustCompletionListConcurrencyCountInformation, // s: in ULONG
	AlpcRegisterCallbackInformation, // kernel-mode only
	AlpcCompletionListRundownInformation, // s: VOID
	AlpcWaitForPortReferences
} ALPC_PORT_INFORMATION_CLASS;

typedef struct _ALPC_SERVER_INFORMATION
{
	union
	{
		struct
		{
			HANDLE ThreadHandle;
		} In;
		struct
		{
			BOOLEAN ThreadBlocked;
			HANDLE ConnectedProcessId;
			UNICODE_STRING ConnectionPortName;
		} Out;
	};
} ALPC_SERVER_INFORMATION, * PALPC_SERVER_INFORMATION;

typedef struct _ALPC_BASIC_INFORMATION
{
	ULONG Flags;
	ULONG SequenceNo;
	PVOID PortContext;
} ALPC_BASIC_INFORMATION, * PALPC_BASIC_INFORMATION;

typedef enum _ALPC_MESSAGE_INFORMATION_CLASS {
	AlpcMessageSidInformation, // out PSID
	AlpcMessageTokenModifiedIdInformation,  // q: out LUID
	AlpcMessageDirectStatusInformation,
	AlpcMessageHandleInformation, // ALPC_MESSAGE_HANDLE_INFORMATION
	MaxAlpcMessageInfoClass
} ALPC_MESSAGE_INFORMATION_CLASS, * PALPC_MESSAGE_INFORMATION_CLASS;

typedef struct _ALPC_MESSAGE_HANDLE_INFORMATION {
	ULONG Index;
	ULONG Flags;
	ULONG Handle;
	ULONG ObjectType;
	ACCESS_MASK GrantedAccess;
} ALPC_MESSAGE_HANDLE_INFORMATION, * PALPC_MESSAGE_HANDLE_INFORMATION;

////
//
// FUNCTIONS
//
////

EXTERN_C
LPVOID NTAPI AlpcGetMessageAttribute(
	_In_ PALPC_MESSAGE_ATTRIBUTES Buffer,
	_In_ ULONG AttributeFlag
);

EXTERN_C
NTSTATUS NTAPI AlpcInitializeMessageAttribute(
	_In_ ULONG AttributeFlags,
	_Out_opt_ PALPC_MESSAGE_ATTRIBUTES Buffer,
	_In_ SIZE_T BufferSize,
	_Out_ PSIZE_T RequiredBufferSize
);

EXTERN_C
NTSTATUS NTAPI NtAlpcCreatePort(
	_Out_ PHANDLE PortHandle,
	_In_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_opt_ PALPC_PORT_ATTRIBUTES PortAttributes
);

EXTERN_C
NTSTATUS NTAPI NtAlpcConnectPort(
	_Out_ PHANDLE PortHandle,
	_In_ PUNICODE_STRING PortName,
	_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_opt_ PALPC_PORT_ATTRIBUTES PortAttributes,
	_In_ DWORD ConnectionFlags,
	_In_opt_ PSID RequiredServerSid,
	_In_opt_ PPORT_MESSAGE ConnectionMessage,
	_Inout_opt_ PSIZE_T ConnectMessageSize,
	_In_opt_ PALPC_MESSAGE_ATTRIBUTES OutMessageAttributes,
	_In_opt_ PALPC_MESSAGE_ATTRIBUTES InMessageAttributes,
	_In_opt_ PLARGE_INTEGER Timeout
);

EXTERN_C
NTSTATUS NTAPI NtAlpcSendWaitReceivePort(
    _In_ HANDLE PortHandle,
    _In_ DWORD Flags,
    _In_opt_ PPORT_MESSAGE SendMessage_,
    _In_opt_ PALPC_MESSAGE_ATTRIBUTES SendMessageAttributes,
    _Inout_opt_ PPORT_MESSAGE ReceiveMessage,
    _Inout_opt_ PSIZE_T BufferLength,
    _Inout_opt_ PALPC_MESSAGE_ATTRIBUTES ReceiveMessageAttributes,
    _In_opt_ PLARGE_INTEGER Timeout
);

EXTERN_C
NTSTATUS NTAPI NtAlpcAcceptConnectPort(
	_Out_ PHANDLE PortHandle,
	_In_ HANDLE ConnectionPortHandle,
	_In_ DWORD Flags,
	_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_opt_ PALPC_PORT_ATTRIBUTES PortAttributes,
	_In_opt_ PVOID PortContext,
	_In_ PPORT_MESSAGE ConnectionRequest,
	_Inout_opt_ PALPC_MESSAGE_ATTRIBUTES ConnectionMessageAttributes,
	_In_ BOOLEAN AcceptConnection
);

EXTERN_C
SIZE_T NTAPI AlpcGetHeaderSize(
	ULONG TypeFlag
);

EXTERN_C
NTSTATUS NTAPI NtAlpcImpersonateClientOfPort(
	_In_ HANDLE         PortHandle,
	_In_ PPORT_MESSAGE  PortMessage,
	_Reserved_ PVOID    Reserved
);

EXTERN_C
NTSTATUS NTAPI NtAlpcImpersonateClientContainerOfPort(
	_In_ HANDLE PortHandle,
	_In_ PPORT_MESSAGE Message,
	_In_ ULONG Flags
);

EXTERN_C
NTSTATUS NTAPI NtAlpcDisconnectPort(
	_In_ HANDLE PortHandle,
	_In_ ULONG Flags
);

EXTERN_C
NTSTATUS NTAPI NtAlpcCreatePortSection(
	_In_ HANDLE PortHandle,
	_In_ ULONG Flags,
	_In_opt_ HANDLE SectionHandle,
	_In_ SIZE_T SectionSize,
	_Out_ PHANDLE AlpcSectionHandle,
	_Out_ PSIZE_T ActualSectionSize
);
EXTERN_C
NTSTATUS NTAPI NtAlpcCreateSectionView(
	_In_ HANDLE PortHandle,
	_Reserved_ ULONG Flags,
	_Inout_ PALPC_DATA_VIEW_ATTR ViewAttributes
);
EXTERN_C
ULONG NTAPI AlpcMaxAllowedMessageLength();

EXTERN_C
NTSTATUS NTAPI NtAlpcQueryInformationMessage(
	_In_ HANDLE PortHandle,
	_In_ PPORT_MESSAGE PortMessage,
	_In_ ALPC_MESSAGE_INFORMATION_CLASS MessageInformationClass,
	__out_bcount(Length) PVOID MessageInformation,
	_In_ SIZE_T Length,
	_Out_opt_ PSIZE_T ReturnLength
);

// From https://processhacker.sourceforge.io/doc/ntlpcapi_8h.html#a1c4ea76677db393f9bb13baabdcb242a
EXTERN_C
NTSTATUS NTAPI NtAlpcCreateSecurityContext(
	_In_ HANDLE  	PortHandle,
	_Reserved_ ULONG  	Flags,
	_Inout_ PALPC_SECURITY_ATTR  	SecurityAttribute
);

//EXTERN_C
//NTSTATUS NTAPI NtAlpcOpenSenderThread(
//	_Out_ PHANDLE ThreadHandle,
//	_In_ HANDLE PortHandle,
//	_In_ PPORT_MESSAGE PortMessage,
//	_In_ ULONG Flags,
//	_In_ ACCESS_MASK DesiredAccess,
//	_In_ POBJECT_ATTRIBUTES ObjectAttributes
//);

////
//
// CUSTOM Additions for my specific ALPC serer
//
////

typedef struct _CS_PORT_CONTEXT {
	ULONG PID;
	ULONG TID;
	ULONG ID;
} CS_PORT_CONTEXT, * PCS_PORT_CONTEXT;


typedef struct _ALPC_MESSAGE {
	PORT_MESSAGE PortHeader;
	BYTE PortMessage[1000]; // Hard limit for this is 65488. An Error is thrown if AlpcMaxAllowedMessageLength() is exceeded
} ALPC_MESSAGE, * PALPC_MESSAGE;

////
//
// OTHER Stuff
//
////

//typedef struct _PH_TOKEN_ATTRIBUTES
//{
//	HANDLE TokenHandle;
//	struct
//	{
//		ULONG Elevated : 1;
//		ULONG ElevationType : 2;
//		ULONG ReservedBits : 29;
//	};
//	ULONG Reserved;
//	PSID TokenSid;
//} PH_TOKEN_ATTRIBUTES, * PPH_TOKEN_ATTRIBUTES;


//0x4 bytes (sizeof)
struct _EX_PUSH_LOCK
{
	union
	{
		struct
		{
			ULONG Locked : 1;                                                 //0x0
			ULONG Waiting : 1;                                                //0x0
			ULONG Waking : 1;                                                 //0x0
			ULONG MultipleShared : 1;                                         //0x0
			ULONG Shared : 28;                                                //0x0
		};
		ULONG Value;                                                        //0x0
		PVOID Ptr;                                                          //0x0
	};
};

typedef struct _ALPC_HANDLE_TABLE{
	_ALPC_HANDLE_ENTRY Handles;
	UINT TotalHandles;
	UINT Flags;
	_EX_PUSH_LOCK Lock;
} ALPC_HANDLE_TABLE, * PALPC_HANDLE_TABLE;


//typedef struct _EPROCESS* PEPROCESS;
typedef struct _KALPC_SECURITY_DATA
{
	PALPC_HANDLE_TABLE HandleTable;	//_KALPC_SECURITY_DATA
	LPVOID ContextHandle;
	LPVOID OwningProcess;
	LPVOID OwnerPort;
	_SECURITY_CLIENT_CONTEXT DynamicSecurity;
} KALPC_SECURITY_DATA, * PKALPC_SECURITY_DATA;

