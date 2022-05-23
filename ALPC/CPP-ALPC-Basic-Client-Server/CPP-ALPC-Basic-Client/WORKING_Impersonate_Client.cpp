
//#include <iostream>
//#include "..\CPP-ALPC-Basic-Server\ALPC.h"
//#include <winnt.h>
//#include <sddl.h>
//
//#pragma comment( lib, "ntdll" ) // This could also be defined in the project properties, but i wanted to ensure everyone spots that you need to link against ntdll.lib
//
//const long MAX_MESSAGE_SIZE = 0x100;
//
//int main() {
//	HANDLE hSrvCommPort;
//	NTSTATUS lSuccess;
//	PORT_MESSAGE pmReceived;
//	PORT_MESSAGE pmSend;
//	WCHAR wcMessage[MAX_MESSAGE_SIZE];
//	LPVOID lpHeapMem;
//	ULONG pmReceivedSize;
//	ULONG ulMessageID = 0, ulCallbackID = 0;
//	UNICODE_STRING usPortName;
//	LPCWSTR pwsPortName = L"\\RPC Control\\FALPCPort"; // the name of the ALPC port we're attempting to connect to 
//
//	DWORD dwPID = GetCurrentProcessId();
//	DWORD dwTID = GetCurrentThreadId();
//	wprintf(L"[*] Starting Client. PID: %d | TID: %d\n", dwPID, dwTID);
//	CLIENT_ID clientID;
//	clientID.UniqueProcess = &dwPID;
//	clientID.UniqueThread = &dwTID;
//
//	RtlInitUnicodeString(&usPortName, pwsPortName);	// Initialize Unicode String
//
//	// IS THIS NEEDED??
//	//OBJECT_ATTRIBUTES objAttributes;
//	//InitializeObjectAttributes(						// set up OBJECT_ATTRIBUTES structure
//	//	&objAttributes,		// the pointer to the OBJECT_ATTRIBUTES structure
//	//	&usPortName,	// the name the object we want to obtain a handle for
//	//	0,					// no flags, ref.: https://docs.microsoft.com/en-us/windows/win32/api/ntdef/nf-ntdef-initializeobjectattributes
//	//	NULL,				// no root directry handle as our port name is a full qualified path
//	//	NULL				// no security descriptor
//	//);
//
//	// QOS
//	SECURITY_QUALITY_OF_SERVICE SecurityQos;
//	SecurityQos.ImpersonationLevel = SecurityImpersonation;
//	SecurityQos.ContextTrackingMode = SECURITY_STATIC_TRACKING;
//	SecurityQos.EffectiveOnly = 0;
//	SecurityQos.Length = sizeof(SecurityQos);
//	// ALPC Port Attributs
//	ALPC_PORT_ATTRIBUTES PortAttributes;
//	PortAttributes.Flags = 0;// 0x8080000; // ALPC_PORFLG_ALLOW_LPC_REQUESTS;// | ALPC_PORFLG_ACCEPT_DUP_HANDLES | ALPC_PORFLG_ACCEPT_INDIRECT_HANDLES;//ALPC_PORFLG_ALLOW_LPC_REQUESTS;
//	PortAttributes.MaxMessageLength = sizeof(ALPC_MESSAGE);
//	PortAttributes.MemoryBandwidth = 0;
//	PortAttributes.MaxPoolUsage = 0x4000;
//	PortAttributes.MaxSectionSize = 0;
//	PortAttributes.MaxTotalSectionSize = 0;
//	PortAttributes.MaxViewSize = 0;
//	PortAttributes.DupObjectTypes = 0xffffffff;
//	//PortAttributes.DupObjectTypes = 0xffffffff;//OB_FILE_OBJECT_TYPE;
//	PortAttributes.SecurityQos = SecurityQos;
//
//	// connection message
//	//PORT_MESSAGE pmConnectionMessage;
//	//WCHAR wcConnectionMsg[MAX_MESSAGE_SIZE] = L"Hello";
//	//ULONG ulConnctMsgSize = wcslen(wcConnectionMsg);
//	//pmConnectionMessage.u1.s1.DataLength = ulConnctMsgSize;
//	//pmConnectionMessage.u1.s1.TotalLength = sizeof(PORT_MESSAGE) + pmConnectionMessage.u1.s1.DataLength;
//	////-- Allocate local HEAP to hold the PORT_MESSAGE structure + your actual Message
//	//lpHeapMem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PORT_MESSAGE) + pmConnectionMessage.u1.s1.DataLength);
//	////-- Copy PORT_MESSAGE Structure + your message to your allocated local HEAP
//	//RtlMoveMemory(lpHeapMem, &pmConnectionMessage, sizeof(PORT_MESSAGE));
//	//RtlMoveMemory((BYTE*)lpHeapMem + sizeof(PORT_MESSAGE), &wcConnectionMsg, pmConnectionMessage.u1.s1.DataLength);
//
//	//ALPC_MESSAGE_ATTRIBUTES inMsgAttr;
//	//RtlSecureZeroMemory(&inMsgAttr, sizeof(inMsgAttr));
//	//UCHAR ConnectionMessageAttributesBuffer[100];
//	//PALPC_MESSAGE_ATTRIBUTES inMsgAttr = (PALPC_MESSAGE_ATTRIBUTES)&ConnectionMessageAttributesBuffer;
//	//ALPC_MESSAGE_ATTRIBUTES outMsgAttr;
//	//PALPC_MESSAGE_ATTRIBUTES inMsgAttr;
//	SIZE_T ConnectionMessageAttributesBufferLength;
//	ULONG ulMsgAttr = ALPC_MESSAGE_ALL; // ALPC_MESSAGE_CONTEXT_ATTRIBUTE;//0x70000000;//0x0E2000000; // ALPC_MESSAGE_HANDLE_ATTRIBUTE;
//	ULONG ulAttrSize = AlpcGetHeaderSize(ulMsgAttr);
//	// Allocate Memory for ALPC Attributes
//	//	Attributes will be placed in process memory after ALPC_MESSAGE_ATTRIBUTES structure by kernel
//	LPVOID lpMemALPCAttrIn = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ALPC_MESSAGE_ATTRIBUTES) + ulAttrSize);
//	PALPC_MESSAGE_ATTRIBUTES pALPCMsgAttrIn = (PALPC_MESSAGE_ATTRIBUTES)lpMemALPCAttrIn;
//	if (GetLastError() != 0) {
//		wprintf(L"[-] Failed to allocate memory for ALPC Message attributes.");
//		return 1;
//	}
//	wprintf(L"[*] Initializing ReceiveMessage Attributes (0x%X)...", ulMsgAttr);
//	lSuccess = AlpcInitializeMessageAttribute(
//		ulMsgAttr,	// attributes
//		pALPCMsgAttrIn,	// pointer to attributes structure
//		ConnectionMessageAttributesBufferLength,	// buffer size
//		(PULONG)&ConnectionMessageAttributesBufferLength
//	);
//	if (!NT_SUCCESS(lSuccess)) {
//		wprintf(L"Error: 0x%X\n", lSuccess);
//		return 1;
//	}
//	else wprintf(L"Done.\n");
//	wprintf(L"[*] Connecting to port '%s'...", pwsPortName);
//	ALPC_MESSAGE sendMessage1;
//	RtlSecureZeroMemory(&sendMessage1, sizeof(sendMessage1));
//	sendMessage1.PortHeader.u1.s1.TotalLength = sizeof(PORT_MESSAGE);
//	sendMessage1.PortHeader.u1.s1.DataLength = 0; // 0 since we don't send any additional data to the server
//	sendMessage1.PortHeader.ClientId.UniqueProcess = 0;
//	sendMessage1.PortHeader.ClientId.UniqueThread = 0;
//	SIZE_T receivedLen1 = sizeof(sendMessage1);
//	//SID
//	PSID pSID;
//	ConvertStringSidToSid(
//		L"S-1-5-21-258271829-1673813934-3670761721-1000",
//		&pSID
//	);
//
//
//	ALPC_PORT_ATTRIBUTES apa;
//	SECURITY_QUALITY_OF_SERVICE sqos;
//	RtlZeroMemory(&sqos, sizeof(SECURITY_QUALITY_OF_SERVICE));
//	InitializeAlpcPortAttributes(&apa, 0x10000, SecurityQos, 256, 128, 128, 512, sizeof(PORT_VIEW), 512, 0);
//
//	lSuccess = NtAlpcConnectPort(
//		&hSrvCommPort,		// port handle
//		&usPortName,	// the port name we want to connect to
//		NULL,			// no object attributes
//		&apa, //&PortAttributes,			// no port attributes
//		ALPC_MSGFLG_SYNC_REQUEST,				// no Flags
//		pSID,			// no required SID
//		(PPORT_MESSAGE)&sendMessage1, //(PPORT_MESSAGE)lpHeapMem,			// no connection message
//		(PULONG)&receivedLen1, //&ulConnctMsgSize,			// no connection message size
//		NULL,			// no out messages attribtus
//		pALPCMsgAttrIn,			// no in message attributes
//		NULL			// no timeout
//	);
//	if (lSuccess == STATUS_PORT_CONNECTION_REFUSED) {
//		wprintf(L"\n[*] The Server denied the connection request.\n");
//		return 0;
//	}
//	else if (!NT_SUCCESS(lSuccess)) {
//		wprintf(L"Error: 0x%X\n", lSuccess);
//		return 1;
//	}
//	else wprintf(L"Success. Client Connection Port: 0x%X\n", hSrvCommPort);
//	// accept = 0xB ?
//	wprintf(L"\tConnected (Type: 0x%X) to Proccess PID '%d' (ThreadID: %d).\n",
//		sendMessage1.PortHeader.u2.s2.Type,
//		sendMessage1.PortHeader.ClientId.UniqueProcess,
//		sendMessage1.PortHeader.ClientId.UniqueThread
//	);
//	if (sendMessage1.PortHeader.u1.s1.DataLength != 0) {
//		wprintf(L"\tConnection Message: ");
//		for (int i = 0; i <= sendMessage1.PortHeader.u1.s1.DataLength; i++) {
//			BYTE* pbyReadPointer = (BYTE*)sendMessage1.PortMessage + i;
//			BYTE byRead = *pbyReadPointer;
//			printf("%c", byRead);
//		}
//		wprintf(L"\n");
//	};
//
//	//ALPC_MESSAGE_HANDLE_INFORMATION HandleInfo;
//	//HandleInfo.Index = 0;
//	//lSuccess = NtAlpcQueryInformationMessage(hSrvCommPort,
//	//	(PPORT_MESSAGE)&sendMessage1,
//	//	AlpcMessageHandleInformation,
//	//	&HandleInfo,
//	//	sizeof(HandleInfo),
//	//	NULL
//	//);
//	//if (!NT_SUCCESS(lSuccess)) {
//	//	wprintf(L"Error: 0x%X\n", lSuccess);
//	//	//return 1;
//	//}
//	//else wprintf(L"Success.\n");
//
//	// Get CONTEXT ATTR
//	//ALPC_MESSAGE_ATTRIBUTES inMsgAttr = *pALPCMsgAttrIn;
//	//if (inMsgAttr.ValidAttributes == ALPC_MESSAGE_CONTEXT_ATTRIBUTE) {
//	//	LPVOID lpMessageAttr = AlpcGetMessageAttribute(pALPCMsgAttrIn, ALPC_MESSAGE_CONTEXT_ATTRIBUTE);
//	//	ALPC_CONTEXT_ATTR ALPCContextAttrIn = *(PALPC_CONTEXT_ATTR)lpMessageAttr;
//	//	//ALPC_CONTEXT_ATTR ALPCContextAttrIn = *(PALPC_CONTEXT_ATTR)((BYTE*)pALPCMsgAttrIn + sizeof(ALPC_MESSAGE_ATTRIBUTES));
//	//	ulMessageID = ALPCContextAttrIn.MessageId;
//	//	ulCallbackID = ALPCContextAttrIn.CallbackId;
//	//	wprintf(L"Got Message ID: %d.\n", ulMessageID);
//	//}
//	//
//	//HeapFree(GetProcessHeap(), 0, lpMemALPCAttrIn);
//
//	//wprintf(L"Waiting for reply from server...");
//	//ALPC_MESSAGE_ATTRIBUTES test;
//	//ULONG bufferLen;
//	//lSuccess = AlpcInitializeMessageAttribute(
//	//	ALPC_MESSAGE_CONTEXT_ATTRIBUTE,	// attributes
//	//	&test,	// pointer to attributes structure
//	//	AlpcGetHeaderSize(ALPC_MESSAGE_CONTEXT_ATTRIBUTE),	// buffer size
//	//	&bufferLen
//	//);
//	//lSuccess = NtAlpcSendWaitReceivePort(
//	//	hSrvCommPort,				// our port handle
//	//	0,				// no Flags
//	//	NULL,	// client consisting of PORT_MESSAGE struct + actual message
//	//	NULL, // no send attributes
//	//	NULL,//&pmReceived, // no receive buffer
//	//	NULL,//&pmReceivedSize,	// no buffer length pointer
//	//	&test, // no receive message attributes
//	//	NULL // no timeout
//	//);
//	//if (!NT_SUCCESS(lSuccess)) {
//	//	wprintf(L"Error: 0x%X\n", lSuccess);
//	//	return 1;
//	//}
//	//else wprintf(L"Success.\n");
//
//	//hSrvCommPort = (HANDLE)((int)hSrvConnectPort + 0x4); // UNTRUE
//
//	//LPVOID lpMessageAttr3 = AlpcGetMessageAttribute(&test, ALPC_MESSAGE_VIEW_ATTRIBUTE);
//	//LPVOID lpMessageAttr2 = AlpcGetMessageAttribute(&test, ALPC_MESSAGE_HANDLE_ATTRIBUTE);
//	//LPVOID lpMessageAttr = AlpcGetMessageAttribute(&test, ALPC_MESSAGE_CONTEXT_ATTRIBUTE);
//	//ALPC_CONTEXT_ATTR contextAttr;
//	//contextAttr = *(PALPC_CONTEXT_ATTR)lpMessageAttr;
//	//hSrvCommPort = contextAttr.MessageContext;
//
//	// get communication port
//	//HandleAttributes = ALPC_GET_HANDLE_ATTRIBUTES(inMsgAttr);
//
//	// get information
//	//wprintf(L"Get Server Information");
//	//ULONG ulAllocatedSize = sizeof(ALPC_SERVER_INFORMATION);
//	//PVOID pPortInformation = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulAllocatedSize);
//	//ALPC_SERVER_INFORMATION foo;
//	////foo.In.ThreadHandle = GetCurrentThread();
//	//ULONG ulLength = 0;
//	//lSuccess = NtAlpcQueryInformation(
//	//	hSrvCommPort,	// port handle
//	//	AlpcServerInformation,	// information class
//	//	&foo,	// pointer to port information
//	//	sizeof(foo),					// length of port information
//	//	&ulLength		// actual size required for the returned structure
//	//);
//	//if (!NT_SUCCESS(lSuccess)) {
//	//	wprintf(L"Error: 0x%X\n", lSuccess);
//	//	//return 1;
//	//}
//	//else wprintf(L"Success.\n");
//
//	//wprintf(L"Getting communication port....");
//	//LPVOID foobar = AlpcGetMessageAttribute(&inMsgAttr, ALPC_MESSAGE_HANDLE_ATTRIBUTE);
//	//ALPC_CONTEXT_ATTR clientContextAttr;
//	//hSrvCommPort = nullptr;
//	//if (inMsgAttr.ValidAttributes == ALPC_MESSAGE_CONTEXT_ATTRIBUTE) {
//	//	clientContextAttr = *(PALPC_CONTEXT_ATTR)foobar;
//	//	hSrvCommPort = clientContextAttr.MessageContext;
//	//}
//	//else {
//	//	wprintf(L"Failed (0x%X).\n", inMsgAttr.ValidAttributes);
//	//}
//	// Send message to server
//	ALPC_MESSAGE sendMessage;
//	RtlSecureZeroMemory(&sendMessage, sizeof(sendMessage));
//	LPVOID lpPortMessage = &(sendMessage.PortMessage);
//	char msg[100] = "Hello ALPC Server";
//	int lMsgLen = strlen(msg);
//	memmove(lpPortMessage, msg, lMsgLen);
//	sendMessage.PortHeader.u1.s1.DataLength = lMsgLen;
//	sendMessage.PortHeader.u1.s1.TotalLength = sizeof(PORT_MESSAGE) + lMsgLen;
//	//sendMessage.PortHeader.MessageId = ulMessageID;
//	//sendMessage.PortHeader.CallbackId = ulCallbackID;
//	//sendMessage.PortHeader.ClientId.UniqueProcess = 0;	// should be filled in by kernel
//	//sendMessage.PortHeader.ClientId.UniqueThread = 0;	// should be filled in by kernel
//	//// -- Prepare the message buffers -- OLD WAY
//	//RtlSecureZeroMemory(&pmSend, sizeof(pmSend));
//	//RtlSecureZeroMemory(&wcMessage, sizeof(wcMessage));
//	//// -- Fill message buffer
//	//wprintf(L"[.] Enter Message for to send (max. %d chars): ", MAX_MESSAGE_SIZE);
//	//std::wcin.getline(wcMessage, MAX_MESSAGE_SIZE);
//	//// -- Set up the PORT_MESSAGE structure; Your message will be appended to the PORT_MESSAGE structure
//	//pmSend.u1.s1.DataLength = wcslen(wcMessage);
//	//pmSend.u1.s1.TotalLength = sizeof(PORT_MESSAGE) + pmSend.u1.s1.DataLength;
//	//pmSend.MessageId = 0;
//	//pmSend.u2.ZeroInit = 0;
//	//pmSend.ClientId.UniqueProcess = 0;
//	//pmSend.ClientId.UniqueThread = 0;
//	////-- Allocate local HEAP to hold the PORT_MESSAGE structure + your actual Message
//	//lpHeapMem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PORT_MESSAGE) + pmSend.u1.s1.DataLength);
//	////-- Copy PORT_MESSAGE Structure + your message to your allocated local HEAP
//	//RtlMoveMemory(lpHeapMem, &pmSend, sizeof(PORT_MESSAGE));
//	//RtlMoveMemory( (BYTE*)lpHeapMem + sizeof(PORT_MESSAGE), &wcMessage, pmSend.u1.s1.DataLength);
//	//// -- Specify message attributes
//	//// Message Attributes
//	//ALPC_MESSAGE_ATTRIBUTES SendMessageAttributes, ReceiveMessageAttributes;
//	//ULONG ulRequiredBufferSize;
//	//ULONG loobar;
//	//ULONG ulALPCReqAttr = ALPC_MESSAGE_SECURITY_ATTRIBUTE | ALPC_MESSAGE_CONTEXT_ATTRIBUTE | ALPC_MESSAGE_TOKEN_ATTRIBUTE; //| 0x08000000;// 0x08000000;//ALPC_MESSAGE_SECURITY_ATTRIBUTE;
//	//ULONG ulAlpcAttrBuf = AlpcGetHeaderSize(ulALPCReqAttr);
//	//
//	//wprintf(L"[*] Initializing ReceiveMessage Attributes (0x%X)...", ulALPCReqAttr);
//	//lSuccess = AlpcInitializeMessageAttribute(
//	//	ulALPCReqAttr,	// attributes
//	//	&SendMessageAttributes,	// pointer to attributes structure
//	//	ulAlpcAttrBuf,	// buffer size
//	//	&ulRequiredBufferSize
//	//);
//	//if (!NT_SUCCESS(lSuccess)) {
//	//	wprintf(L"Error: 0x%X\n", lSuccess);
//	//	return 1;
//	//}
//	//else wprintf(L"Done.\n");
//	// -- Sending the Message
//	SIZE_T ConnectionMessageAttributesBufferLength2;
//	ULONG ulMsgAttr2 = ALPC_MESSAGE_ALL; // ALPC_MESSAGE_ALL;//0x70000000;//0x0E2000000; // ALPC_MESSAGE_HANDLE_ATTRIBUTE;
//	ULONG ulAttrSize2 = AlpcGetHeaderSize(ulMsgAttr2);
//	LPVOID lpMemALPCAttrOut = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ALPC_MESSAGE_ATTRIBUTES) + ulAttrSize2);
//	PALPC_MESSAGE_ATTRIBUTES pALPCMsgAttrOut = (PALPC_MESSAGE_ATTRIBUTES)lpMemALPCAttrOut;
//	if (GetLastError() != 0) {
//		wprintf(L"[-] Failed to allocate memory for ALPC Message attributes.");
//		return 1;
//	}
//	wprintf(L"[*] Initializing ReceiveMessage Attributes (0x%X)...", ulMsgAttr2);
//	lSuccess = AlpcInitializeMessageAttribute(
//		ulMsgAttr2,	// attributes
//		pALPCMsgAttrOut,	// pointer to attributes structure
//		ulAttrSize2,	// buffer size
//		(PULONG)&ConnectionMessageAttributesBufferLength2
//	);
//	if (!NT_SUCCESS(lSuccess)) {
//		wprintf(L"Error: 0x%X\n", lSuccess);
//		return 1;
//	}
//	else wprintf(L"Success.\n");
//	//Sleep(3000);
//	ALPC_MESSAGE recvMessage;
//	RtlSecureZeroMemory(&recvMessage, sizeof(recvMessage));
//	SIZE_T recvMsgLen = sizeof(recvMessage);
//	wprintf(L"[*] [PID: %d; TID: %d] Sending Message (len: %d) to ALPC Port 0x%X....", GetCurrentProcessId(), GetCurrentThreadId(), sendMessage.PortHeader.u1.s1.DataLength, hSrvCommPort);
//	lSuccess = NtAlpcSendWaitReceivePort(
//		hSrvCommPort,				// our port handle
//		NULL,				// no Flags
//		(PPORT_MESSAGE)&sendMessage,	// client consisting of PORT_MESSAGE struct + actual message
//		pALPCMsgAttrOut,//&SendMessageAttributes, // no send attributes
//		NULL,//(PPORT_MESSAGE)&recvMessage,//&pmReceived, // no receive buffer
//		NULL,//(PULONG)&recvMsgLen, //&ulBuf,	// no buffer length pointer
//		NULL,//&alpcma, // no receive message attributes
//		NULL // no timeout
//	);
//	if (!NT_SUCCESS(lSuccess)) {
//		wprintf(L"Error: 0x%X\n", lSuccess);
//		return 1;
//	}
//	else wprintf(L"Success.\n");
//	//HeapFree(GetProcessHeap,0, lpHeapMem); // Free used HEAP memory
//	//Close connection
//	wprintf(L"[*] Closing Port connection...");
//	NtAlpcDisconnectPort(hSrvCommPort, 0);
//	if (!NT_SUCCESS(lSuccess)) {
//		wprintf(L"Error: 0x%X\n", lSuccess);
//		return 1;
//	}
//	else wprintf(L"Success.\n");
//	CloseHandle(hSrvCommPort);
//	return 0;
//}
//
//