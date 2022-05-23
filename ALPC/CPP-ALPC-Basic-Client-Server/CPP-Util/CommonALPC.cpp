#include "psapi.h"
#include "ALPC.h"
#include "Command.cpp"

// This could also be defined in the project properties, but i wanted to ensure everyone spots that you need to link against those libs
#pragma comment( lib, "ntdll" )
#pragma comment( lib, "User32" )

ALPC_MESSAGE_TYPE get_message_type(ULONG ulType) {
	ULONG ulConverted = ulType & 0xFF;
	return ALPC_MESSAGE_TYPE(ulConverted);
}

LPCWSTR message_type_to_sz(ULONG ulType) {
	ALPC_MESSAGE_TYPE mType = get_message_type(ulType);
	switch (mType)
	{
	case None:   return L"None";
	case Request:   return L"Request";
	case Reply: return L"Reply";
	case Datagram: return L"Datagram";
	case LostReply: return L"LostReply";
	case PortClosed: return L"PortClosed";
	case ClientDied: return L"ClientDied";
	case Exception: return L"Exception";
	case DebugEvent: return L"DebugEvent";
	case ErrorEvent: return L"ErrorEvent";
	case ConnectionRequest: return L"ConnectionRequest";
	case ConnectionReply: return L"ConnectionReply";
	case UNKNOWN12: return L"UNKNOWN12";
	case PortDisconnected: return L"PortDisconnected";
	default:      return L"Unknown";
	}
}

VOID print_port_message(ALPC_MESSAGE portMessage) {
	for (int i = 0; i <= portMessage.PortHeader.u1.s1.DataLength; i++) {
		BYTE* pbyReadPointer = (BYTE*)portMessage.PortMessage + i;
		BYTE byRead = *pbyReadPointer;
		printf("%c", byRead);
	}
	wprintf(L"\n");
}

VOID print_clientSID_from_alpc_message(HANDLE hConnectionPort, PPORT_MESSAGE pPortMessage) {
	NTSTATUS lSuccess;
	LPVOID pAlloced;
	SIZE_T ulRequiredSize;
	SIZE_T ulAllocedSize = 0x1c;
	// Allow memory for SID
	pAlloced = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulAllocedSize);

	lSuccess = NtAlpcQueryInformationMessage(
		hConnectionPort,
		pPortMessage,
		ALPC_MESSAGE_INFORMATION_CLASS::AlpcMessageSidInformation,
		pAlloced,
		ulAllocedSize,
		&ulRequiredSize
	);
	if (NT_SUCCESS(lSuccess)) {
		PSID pSID = (PSID)pAlloced;
		LPWSTR szSID;
		ConvertSidToStringSid(
			pSID,
			&szSID
		);
		wprintf(L"%s\n", szSID);
	}
}

NTSTATUS impersonate_client(HANDLE hPort, ALPC_MESSAGE pmReceived) {
	BOOL bSuccess;

	// usually not needed for local impersonation
	/*setWindowAccess();
	setDesktopAccess();*/

	wchar_t command[] = L"C:\\Windows\\System32\\cmd.exe";
	bSuccess = execImpersonatedWindowCommand(hPort, pmReceived, command);
	return bSuccess;
}

VOID add_request_message(PALPC_MESSAGE pMessage, LPCSTR messageContent) {
	//// Add a message
	//LPVOID lpPortMessage = &(pMessage.PortMessage);
	////int lMsgLen = wcslen(messageContent);
	//char msg[100] = "Hello Client";
	//int lMsgLen = strlen(msg);

	//memmove(lpPortMessage, msg, lMsgLen);
	//pMessage.PortHeader.u1.s1.DataLength = lMsgLen;
	//pMessage.PortHeader.u1.s1.TotalLength = sizeof(PORT_MESSAGE) + lMsgLen;
	////pMessage.PortHeader.u1.s1.TotalLength = sizeof(pMessage);


	LPVOID lpPortMessage = pMessage->PortMessage;
	//char msg[100] = "Hello Client";
	//int lMsgLen = strlen(msg);
	int lMsgLen = strlen(messageContent);
	memmove(lpPortMessage, messageContent, lMsgLen);
	pMessage->PortHeader.u1.s1.DataLength = lMsgLen;
	pMessage->PortHeader.u1.s1.TotalLength = sizeof(PORT_MESSAGE) + lMsgLen;
}

VOID print_received_alpc_message(ALPC_MESSAGE pmReceived, HANDLE hAlpcPort, BOOL printRequestingSID) {
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, (DWORD)pmReceived.PortHeader.ClientId.UniqueProcess);
	GetProcessImageFileName(hProcess, szProcessName, MAX_PATH);
	// PROCESS CONNECTION
	wprintf(L"[+] Received data!\n\tFom Process PID: %d (Thead TID: %d) [Process: %s]\n\tType: %s\n\tConnectionMessage Length: %d\n",
		pmReceived.PortHeader.ClientId.UniqueProcess,
		pmReceived.PortHeader.ClientId.UniqueThread,
		szProcessName,
		message_type_to_sz(pmReceived.PortHeader.u2.s2.Type),
		pmReceived.PortHeader.u1.s1.DataLength
	);
	if (pmReceived.PortHeader.u1.s1.DataLength > 0) {
		wprintf(L"\tConnecting Message: ");
		print_port_message(pmReceived);
	}
	else { wprintf(L"\n"); }
	if (printRequestingSID) {
		wprintf(L"\tClient SID: ");
		print_clientSID_from_alpc_message(hAlpcPort, (PPORT_MESSAGE)&pmReceived);
	}
}


typedef enum _POOL_TYPE
{
	NonPagedPool,
	PagedPool,
	NonPagedPoolMustSucceed,
	DontUseThisType,
	NonPagedPoolCacheAligned,
	PagedPoolCacheAligned,
	NonPagedPoolCacheAlignedMustS
} POOL_TYPE, * PPOOL_TYPE;

typedef struct _OBJECT_TYPE_INFORMATION
{
	UNICODE_STRING Name;
	ULONG TotalNumberOfObjects;
	ULONG TotalNumberOfHandles;
	ULONG TotalPagedPoolUsage;
	ULONG TotalNonPagedPoolUsage;
	ULONG TotalNamePoolUsage;
	ULONG TotalHandleTableUsage;
	ULONG HighWaterNumberOfObjects;
	ULONG HighWaterNumberOfHandles;
	ULONG HighWaterPagedPoolUsage;
	ULONG HighWaterNonPagedPoolUsage;
	ULONG HighWaterNamePoolUsage;
	ULONG HighWaterHandleTableUsage;
	ULONG InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ULONG ValidAccess;
	BOOLEAN SecurityRequired;
	BOOLEAN MaintainHandleCount;
	USHORT MaintainTypeList;
	POOL_TYPE PoolType;
	ULONG PagedPoolUsage;
	ULONG NonPagedPoolUsage;
} OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;

typedef struct _SYSTEM_HANDLE
{
	ULONG ProcessId;
	BYTE ObjectTypeNumber;
	BYTE Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, * PSYSTEM_HANDLE;


typedef struct _SYSTEM_HANDLE_INFORMATION
{
	ULONG HandleCount;
	SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

VOID print_valid_message_attribues(PALPC_MESSAGE_ATTRIBUTES pMsgAttr, BOOL bIsServer = FALSE) {
	int iFoundFlags = 0;
	wprintf(L"[*] Valid Message Attributes:\n");
	if (pMsgAttr->ValidAttributes & ALPC_MESSAGE_TOKEN_ATTRIBUTE) {
		wprintf(L"\tALPC_MESSAGE_TOKEN_ATTRIBUTE\n");
		ALPC_TOKEN_ATTR tokenAttr = *(PALPC_TOKEN_ATTR)AlpcGetMessageAttribute(pMsgAttr, ALPC_MESSAGE_TOKEN_ATTRIBUTE);
		wprintf(L"\t  TokenId: %lu -- AuthenticationId: %lu -- ModifiedId: %lu \n", tokenAttr.TokenId, tokenAttr.AuthenticationId, tokenAttr.ModifiedId);
		iFoundFlags += ALPC_MESSAGE_TOKEN_ATTRIBUTE;
	}
	if (pMsgAttr->ValidAttributes & ALPC_MESSAGE_SECURITY_ATTRIBUTE) {
		wprintf(L"\tALPC_MESSAGE_SECURITY_ATTRIBUTE\n");
		ALPC_SECURITY_ATTR securitAttr = *(PALPC_SECURITY_ATTR)AlpcGetMessageAttribute(pMsgAttr, ALPC_MESSAGE_SECURITY_ATTRIBUTE);
		iFoundFlags += ALPC_MESSAGE_SECURITY_ATTRIBUTE;
	}
	if (pMsgAttr->ValidAttributes & ALPC_MESSAGE_CONTEXT_ATTRIBUTE) {
		wprintf(L"\tALPC_MESSAGE_CONTEXT_ATTRIBUTE\n");
		if(  bIsServer ){
			ALPC_CONTEXT_ATTR contextAttr = *(PALPC_CONTEXT_ATTR)AlpcGetMessageAttribute(pMsgAttr, ALPC_MESSAGE_CONTEXT_ATTRIBUTE);
			CS_PORT_CONTEXT csPortContext = *(PCS_PORT_CONTEXT)contextAttr.PortContext;
			wprintf(L"\t  Caller ID: %d\n", csPortContext.ID);
		}
		iFoundFlags += ALPC_MESSAGE_CONTEXT_ATTRIBUTE;
	}
	if (pMsgAttr->ValidAttributes & ALPC_MESSAGE_VIEW_ATTRIBUTE) {
		wprintf(L"\tALPC_MESSAGE_VIEW_ATTRIBUTE\n");
		ALPC_DATA_VIEW_ATTR viewAttr = *(PALPC_DATA_VIEW_ATTR)AlpcGetMessageAttribute(pMsgAttr, ALPC_MESSAGE_VIEW_ATTRIBUTE);
		wprintf(L"\t ViewSize: %d\n", viewAttr.ViewSize);
		iFoundFlags += ALPC_MESSAGE_VIEW_ATTRIBUTE;
	}
	if (pMsgAttr->ValidAttributes & ALPC_MESSAGE_HANDLE_ATTRIBUTE) {
		wprintf(L"\tALPC_MESSAGE_HANDLE_ATTRIBUTE\n");
		ALPC_HANDLE_ATTR handleAttr = *(PALPC_HANDLE_ATTR)AlpcGetMessageAttribute(pMsgAttr, ALPC_MESSAGE_HANDLE_ATTRIBUTE);
		// We could Query the object type like this, code snippet taken from: https://github.com/SinaKarvandi/Process-Magics/blob/master/EnumAllHandles/EnumAllHandles/EnumAllHandles.cpp
		//POBJECT_TYPE_INFORMATION OBJECT_TYPE;
		//ULONG GuessSize = 256;
		//ULONG ulRequiredSize;
		//// Allocate Memory
		//OBJECT_TYPE = (POBJECT_TYPE_INFORMATION)VirtualAlloc(NULL, (SIZE_T)GuessSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		//NTSTATUS lSuccess = NtQueryObject(handleAttr.Handle, ObjectTypeInformation, OBJECT_TYPE, GuessSize, &ulRequiredSize);
		// We could read the file like this
		//LPVOID lpAllocTest = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 100);
		//BOOL success = ReadFile(handleAttr.Handle, lpAllocTest, 10, NULL, NULL);
		
		iFoundFlags += ALPC_MESSAGE_HANDLE_ATTRIBUTE;
	}
	if (pMsgAttr->ValidAttributes & ALPC_MESSAGE_DIRECT_ATTRIBUTE) {
		wprintf(L"\tALPC_MESSAGE_DIRECT_ATTRIBUTE\n");
		iFoundFlags += ALPC_MESSAGE_DIRECT_ATTRIBUTE;
	}
	if (pMsgAttr->ValidAttributes & ALPC_MESSAGE_WORK_ON_BEHALF_ATTRIBUTE) {
		wprintf(L"\tALPC_MESSAGE_WORK_ON_BEHALF_ATTRIBUTE\n");
		iFoundFlags += ALPC_MESSAGE_WORK_ON_BEHALF_ATTRIBUTE;
	}
	if (iFoundFlags != pMsgAttr->ValidAttributes) {
		wprintf(L"\t 0x%X Unkwon Message Attribute(s)\n", pMsgAttr->ValidAttributes - iFoundFlags);
	}
}

VOID print_port_flags(ULONG ulPortAttributeFlags) {
	int iFoundFlags = 0;
	wprintf(L"[*] Valid Message Attributes: ");
	if (ALPC_PORTFLG_LPCPORT & ulPortAttributeFlags) {
		wprintf(L"ALPC_PORTFLG_LPCPORT  | ");
		iFoundFlags += ALPC_PORTFLG_LPCPORT;
	}
	if (ALPC_PORTFLG_ALLOWIMPERSONATION & ulPortAttributeFlags) {
		wprintf(L"ALPC_PORTFLG_ALLOWIMPERSONATION | ");
		iFoundFlags += ALPC_PORTFLG_ALLOWIMPERSONATION;
	}
	if (ALPC_PORTFLG_ALLOW_LPC_REQUESTS & ulPortAttributeFlags) {
		wprintf(L"ALPC_PORTFLG_ALLOW_LPC_REQUESTS | ");
		iFoundFlags += ALPC_PORTFLG_ALLOW_LPC_REQUESTS;
	}
	if (ALPC_PORTFLG_WAITABLE_PORT & ulPortAttributeFlags) {
		wprintf(L"ALPC_PORTFLG_WAITABLE_PORT | ");
		iFoundFlags += ALPC_PORTFLG_WAITABLE_PORT;
	}
	if (ALPC_PORTFLG_SYSTEM_PROCESS & ulPortAttributeFlags) {
		wprintf(L"ALPC_PORTFLG_SYSTEM_PROCESS | ");
		iFoundFlags += ALPC_PORTFLG_SYSTEM_PROCESS;
	}
	if (ALPC_PORTFLG_ALLOW_DUP_OBJECT & ulPortAttributeFlags) {
		wprintf(L"ALPC_PORTFLG_ALLOW_DUP_OBJECT | ");
		iFoundFlags += ALPC_PORTFLG_ALLOW_DUP_OBJECT;
	}
	if (ALPC_PORTFLG_LRPC_WAKE_POLICY1 & ulPortAttributeFlags) {
		wprintf(L"ALPC_PORTFLG_LRPC_WAKE_POLICY1 | ");
		iFoundFlags += ALPC_PORTFLG_LRPC_WAKE_POLICY1;
	}
	if (ALPC_PORTFLG_LRPC_WAKE_POLICY2 & ulPortAttributeFlags) {
		wprintf(L"ALPC_PORTFLG_LRPC_WAKE_POLICY2 | ");
		iFoundFlags += ALPC_PORTFLG_LRPC_WAKE_POLICY2;
	}
	if (ALPC_PORTFLG_LRPC_WAKE_POLICY3 & ulPortAttributeFlags) {
		wprintf(L"ALPC_PORTFLG_LRPC_WAKE_POLICY3 | ");
		iFoundFlags += ALPC_PORTFLG_LRPC_WAKE_POLICY3;
	}
	if (ALPC_PORTFLG_DIRECT_MESSAGE & ulPortAttributeFlags) {
		wprintf(L"ALPC_PORTFLG_DIRECT_MESSAGE | ");
		iFoundFlags += ALPC_PORTFLG_DIRECT_MESSAGE;
	}
	
	if (iFoundFlags != ulPortAttributeFlags) {
		wprintf(L"\n[!]\t 0x%X unkwon Port Flag(s)\n", ulPortAttributeFlags - iFoundFlags);
	}
	else {
		wprintf(L"\n");
	}
}

BOOL accept_connection_request() {
	//
	// ACCEPT OR DENY DECISION
	//
	NTSTATUS lSuccess;
	BOOL bAutoAcceptConnection = TRUE;
	BOOL bAcceptConnection;

	if (!bAutoAcceptConnection) {
		wprintf(L"[.] Do you want to accept the connection? [Y]: ");
		WCHAR wcAcceptConnection[2];
		std::wcin.getline(wcAcceptConnection, 2);
		if (wcscmp(wcAcceptConnection, L"Y") == 0 || wcscmp(wcAcceptConnection, L"y") == 0) {
			bAcceptConnection = TRUE;
		}
		else {
			bAcceptConnection = FALSE;
		};
	}
	else { bAcceptConnection = TRUE; }

	return bAcceptConnection;
}

PSECURITY_DESCRIPTOR create_sd_from_string(LPCWSTR szDACL) {
	PSECURITY_DESCRIPTOR pSD;
	ULONG ulSDSize = 0;
	BOOL success = ConvertStringSecurityDescriptorToSecurityDescriptor(
		szDACL,
		SDDL_REVISION_1,
		&pSD,
		&ulSDSize
	);
	return pSD;
}

PALPC_MESSAGE_ATTRIBUTES alloc_message_attribute(ULONG ulAttributeFlags) {
	NTSTATUS lSuccess;
	PALPC_MESSAGE_ATTRIBUTES pAttributeBuffer;
	LPVOID lpBuffer;
	SIZE_T lpReqBufSize;
	SIZE_T ulAllocBufSize;

	ulAllocBufSize = AlpcGetHeaderSize(ulAttributeFlags); // this calculates: sizeof(ALPC_MESSAGE_ATTRIBUTES) + size of attribute structures
	lpBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulAllocBufSize);
	if (GetLastError() != 0) {
		wprintf(L"[-] Failed to allocate memory for ALPC Message attributes.\n");
		return NULL;
	}
	pAttributeBuffer = (PALPC_MESSAGE_ATTRIBUTES)lpBuffer;
	//wprintf(L"[*] Initializing ReceiveMessage Attributes (0x%X)...", ulAttributeFlags);
	lSuccess = AlpcInitializeMessageAttribute(
		ulAttributeFlags,	// attributes
		pAttributeBuffer,	// pointer to attributes structure
		ulAllocBufSize,	// buffer size
		&lpReqBufSize
	);
	if (!NT_SUCCESS(lSuccess)) {
		//wprintf(L"Error: 0x%X\n", lSuccess);
		//pAttributeBuffer->ValidAttributes = ulAttributeFlags;
		return NULL;
	}
	else {
		//wprintf(L"Success.\n");
		return pAttributeBuffer;
	}
}

PALPC_MESSAGE_ATTRIBUTES setup_sample_message_attributes(HANDLE hAlpcPort, HANDLE hSection, ULONG ulMessageAttributes) {
	NTSTATUS lSuccess;
	INT iNextMsgAttrBufferOffset;
	SIZE_T ulReqBufSize;
	SIZE_T ulMessageAttributeBufSize = AlpcGetHeaderSize(ulMessageAttributes);
	LPVOID lpAllocTest = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulMessageAttributeBufSize); // NOTE: This buffer is never freed... for this sample i just don't care for this
	PALPC_MESSAGE_ATTRIBUTES pMsgAttrSend = (PALPC_MESSAGE_ATTRIBUTES)lpAllocTest;

	lSuccess = AlpcInitializeMessageAttribute(
		ulMessageAttributes,			// the MessageAttribute 
		pMsgAttrSend,				// pointer to allocated buffer that is used to holf attributes structures
		ulMessageAttributeBufSize,	// buffer that has been allocated
		&ulReqBufSize				// the size that would be needed (in case of the buffer allocated was too small)
	);
	if (!NT_SUCCESS(lSuccess)) {
		wprintf(L"Error calling AlpcInitializeMessageAttribute: 0x%X\n", lSuccess);
	}
	iNextMsgAttrBufferOffset = 8; // 4 bytes allocated attributes + 4 bytes valid attributes
	
	if (ulMessageAttributes & ALPC_MESSAGE_SECURITY_ATTRIBUTE) {
		// ALPC_MESSAGE_SECURITY_ATTRIBUTE
		SECURITY_QUALITY_OF_SERVICE SecurityQos;
		ALPC_SECURITY_ATTR securityAttr;
		RtlZeroMemory(&securityAttr, sizeof(securityAttr));
		SecurityQos.ImpersonationLevel = SecurityImpersonation; // SecurityIdentification;
		SecurityQos.ContextTrackingMode = SECURITY_STATIC_TRACKING;
		SecurityQos.EffectiveOnly = 0;
		SecurityQos.Length = sizeof(SecurityQos);
		securityAttr.pQOS = &SecurityQos;
		securityAttr.Flags = 0; // 0x10000;
		lSuccess = NtAlpcCreateSecurityContext(hAlpcPort, 0, &securityAttr);
		if (!NT_SUCCESS(lSuccess)) {
			wprintf(L"[-] Error creating security context: 0x%X\n", lSuccess);
		}
		else {
			memmove((PBYTE)pMsgAttrSend + iNextMsgAttrBufferOffset, &securityAttr, sizeof(securityAttr));
			iNextMsgAttrBufferOffset += sizeof(securityAttr);
		}
		//RtlSecureZeroMemory(&securityAttr, sizeof(securityAttr));
		/*securityAttr.ContextHandle = &securityContext;
		securityAttr.Flags = 0;
		securityAttr.pQOS = &SecurityQos;*/
	}
	if (ulMessageAttributes & ALPC_MESSAGE_VIEW_ATTRIBUTE) {
		// ALPC_MESSAGE_VIEW_ATTRIBUTE
		ALPC_DATA_VIEW_ATTR viewAttr;
		viewAttr.Flags = 0;	//unknown
		viewAttr.SectionHandle = hSection;
		viewAttr.ViewBase = 0;
		viewAttr.ViewSize = 20*1024;//50;//sizeof(PORT_VIEW);
		lSuccess = NtAlpcCreateSectionView(
			hAlpcPort, //_In_ HANDLE PortHandle,
			0, // _Reserved_ ULONG Flags,
			&viewAttr //_Inout_ PALPC_DATA_VIEW_ATTR ViewAttributes
		);
		if (!NT_SUCCESS(lSuccess)) {
			wprintf(L"[-] Error creating SectionView: 0x%X\n", lSuccess);
		}
		else {
			// Fill section with some sample junk
			RtlFillMemory(viewAttr.ViewBase, 0x41, 50);
			// place ALPC_MESSAGE_VIEW_ATTRIBUTE structure
			memmove((PBYTE)pMsgAttrSend + iNextMsgAttrBufferOffset, &viewAttr, sizeof(viewAttr));
		};
		iNextMsgAttrBufferOffset += sizeof(viewAttr);
	}
	
	if (ulMessageAttributes & ALPC_MESSAGE_HANDLE_ATTRIBUTE) {
		// ALPC_MESSAGE_HANDLE_ATTRIBUTE
		HANDLE hFile = CreateFile(L"C:\\Users\\Public\\testfile.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ALPC_HANDLE_ATTR handleAttr;
		handleAttr.Handle = hFile;
		handleAttr.ObjectType = 0;
		handleAttr.Flags = 0;
		handleAttr.DesiredAccess = GENERIC_READ;
		memmove((PBYTE)pMsgAttrSend + iNextMsgAttrBufferOffset, &handleAttr, sizeof(handleAttr));
		iNextMsgAttrBufferOffset += sizeof(handleAttr);
	}
	
	if (ulMessageAttributes & ALPC_MESSAGE_HANDLE_ATTRIBUTE) {
		// ALPC_MESSAGE_DIRECT_ATTRIBUTE
		/// not yet implemented
	}

	if (ulMessageAttributes & ALPC_MESSAGE_WORK_ON_BEHALF_ATTRIBUTE) {
		//ALPC_MESSAGE_WORK_ON_BEHALF_ATTRIBUTE
		/// not yet implemented
	}

	return pMsgAttrSend;
}
