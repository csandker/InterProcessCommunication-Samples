// 
// 
// #include <iostream>
// #include "ALPC.h"
// 
// #include "..\CPP-Util\Command.cpp"
// 
// // This could also be defined in the project properties, but i wanted to ensure everyone spots that you need to link against those libs
// #pragma comment( lib, "ntdll" )
// #pragma comment( lib, "User32" )
// 
// 
// NTSTATUS impersonate_client(HANDLE hPort, ALPC_MESSAGE pmReceived) {
// 	BOOL bSuccess;
// 
// 	//wprintf(L"Impersonate from PID: %d; TID: %d", PortMessage->ClientId.UniqueProcess, PortMessage->ClientId.UniqueThread);
// 
// 	//bSuccess = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken2);
// 	//wprintf(L"\nOpenProcess Error: %d\n.", GetLastError());
// 	//ExtractTokenInformation(hToken2);
// 	//for (int i = 0; i <= pmReceived.PortHeader.u1.s1.DataLength; i++) {
// 	//	BYTE* pbyReadPointer = (BYTE*)pmReceived.PortMessage + i;//(BYTE*)((BYTE*)lpHeapMem + i + sizeof(PORT_MESSAGE));
// 	//	BYTE byRead = *pbyReadPointer;
// 	//	printf("%d:%c\n", i, byRead);
// 	//}
// 	//PORT_MESSAGE PortMessage = pmReceived.PortHeader;
// 
// 
// 	setWindowAccess();
// 	setDesktopAccess();
// 
// 	wchar_t command[] = L"C:\\Windows\\System32\\cmd.exe";
// 	bSuccess = execImpersonatedWindowCommand(hPort, pmReceived, command);
// 	return bSuccess;
// 	//	int lua = 4;
// 
// 	//	lSuccess = NtAlpcOpenSenderThread(
// 	//		&hThreadHandle, 
// 	//		hPort, 
// 	//		&PortMessage, //(PPORT_MESSAGE)&pmReceived,
// 	//		ulFlags, 
// 	//		THREAD_IMPERSONATE,
// 	//		NULL
// 	//	);
// 	//	int loo = 4;
// }
// 
// void add_request_message(PALPC_MESSAGE pMessage, LPCSTR messageContent) {
// 	//// Add a message
// 	//LPVOID lpPortMessage = &(pMessage.PortMessage);
// 	////int lMsgLen = wcslen(messageContent);
// 	//char msg[100] = "Hello Client";
// 	//int lMsgLen = strlen(msg);
// 
// 	//memmove(lpPortMessage, msg, lMsgLen);
// 	//pMessage.PortHeader.u1.s1.DataLength = lMsgLen;
// 	//pMessage.PortHeader.u1.s1.TotalLength = sizeof(PORT_MESSAGE) + lMsgLen;
// 	////pMessage.PortHeader.u1.s1.TotalLength = sizeof(pMessage);
// 
// 
// 	LPVOID lpPortMessage = pMessage->PortMessage;
// 	//char msg[100] = "Hello Client";
// 	//int lMsgLen = strlen(msg);
// 	int lMsgLen = strlen(messageContent);
// 	memmove(lpPortMessage, messageContent, lMsgLen);
// 	pMessage->PortHeader.u1.s1.DataLength = lMsgLen;
// 	pMessage->PortHeader.u1.s1.TotalLength = sizeof(PORT_MESSAGE) + lMsgLen;
// }
// 
// int main() {
// 	NTSTATUS lSuccess;
// 	HANDLE hConnectionPort;
// 	HANDLE hCommunicationPort;
// 	OBJECT_ATTRIBUTES objAttributes;
// 	ALPC_PORT_ATTRIBUTES pALPCPAttr;
// 	UNICODE_STRING usAlpcPortName;
// 	//PPORT_MESSAGE pALPCMessage = { 0 };
// 	ALPC_MESSAGE pmReceived;
// 	ALPC_MESSAGE pmRequest;
// 	ULONG pmReceivedSize;
// 	LPCWSTR pwsPortName = L"\\RPC Control\\FALPCPort"; // the name of the ALPC port we're creating
// 
// 	DWORD dwPID = GetCurrentProcessId();
// 	DWORD dwTID = GetCurrentThreadId();
// 	wprintf(L"[*] Starting Server. PID: %d | TID: %d\n", dwPID, dwTID);
// 
// 	RtlInitUnicodeString(&usAlpcPortName, pwsPortName);	// Initialize Unicode String
// 
// 	//PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
// 	PSECURITY_DESCRIPTOR pSD;
// 	ULONG ulSDSize = 0;
// 	ConvertStringSecurityDescriptorToSecurityDescriptor(
// 		L"D:(A;OICI;GA;;;AU)",	// Allow full control to authenticated users
// 		SDDL_REVISION_1,
// 		&pSD,
// 		&ulSDSize
// 	);
// 	InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
// 	InitializeObjectAttributes(						// set up OBJECT_ATTRIBUTES structure
// 		&objAttributes,		// the pointer to the OBJECT_ATTRIBUTES structure
// 		&usAlpcPortName,	// the name the object we want to obtain a handle for
// 		0,					// no flags, ref.: https://docs.microsoft.com/en-us/windows/win32/api/ntdef/nf-ntdef-initializeobjectattributes
// 		NULL,				// no root directry handle as our port name is a full qualified path
// 		NULL //pSD				// no security descriptor
// 	);
// 
// 	// ALPC Port Attributs
// 	ALPC_PORT_ATTRIBUTES PortAttributes;
// 	SECURITY_QUALITY_OF_SERVICE SecurityQos;
// 	SecurityQos.ImpersonationLevel = SecurityImpersonation;
// 	SecurityQos.ContextTrackingMode = SECURITY_STATIC_TRACKING;// SECURITY_DYNAMIC_TRACKING;
// 	SecurityQos.EffectiveOnly = 0;
// 	SecurityQos.Length = sizeof(SecurityQos);
// 
// 	PortAttributes.Flags = ALPC_PORTFLG_AllowImpersonation;//0x8080000;// ALPC_PORFLG_ALLOW_LPC_REQUESTS;// ALPC_PORFLG_ALLOW_LPC_REQUESTS;// | ALPC_PORFLG_SYSTEM_PROCESS;//0x010000 | 0x020000;	// Found '0x3080000' in rpcrt4.dll
// 	PortAttributes.MaxMessageLength = sizeof(ALPC_MESSAGE);
// 	PortAttributes.MemoryBandwidth = 0;
// 	//PortAttributes.MaxPoolUsage = 0xffffffff;
// 	//PortAttributes.MaxSectionSize = 0xffffffff;
// 	//PortAttributes.MaxViewSize = 0xffffffff;
// 	//PortAttributes.MaxTotalSectionSize = 0xffffffff;
// 	//PortAttributes.DupObjectTypes = 0xffffffff;
// 	PortAttributes.SecurityQos = SecurityQos;
// 
// 	//ALPC_PORT_ATTRIBUTES apa;
// 	//SECURITY_QUALITY_OF_SERVICE sqos;
// 	//RtlZeroMemory(&sqos, sizeof(SECURITY_QUALITY_OF_SERVICE));
// 	//InitializeAlpcPortAttributes(&apa, 0x10000, SecurityQos, 256, 128, 128, 512, sizeof(PORT_VIEW), 512, 0);
// 
// 	wprintf(L"[*] Creating ALPC Port '%s'...", pwsPortName);
// 	lSuccess = NtAlpcCreatePort(
// 		&hConnectionPort,			// the handle to our port name
// 		&objAttributes,		// the OBJECT_ATTRIBUTES structure we just initialized
// 		&PortAttributes				// no additional attributes
// 	);
// 	if (!NT_SUCCESS(lSuccess)) {
// 		wprintf(L"Error: 0x%X\n", lSuccess);
// 		return 1;
// 	}
// 	else wprintf(L"Success.\n");
// 	wprintf(L"[*] Server Connection Port: 0x%X\n", hConnectionPort);
// 
// 	wprintf(L"[*] Wait for incoming connections...");
// 	RtlSecureZeroMemory(&pmReceived, sizeof(pmReceived));
// 	//ALPC_MESSAGE_ATTRIBUTES test;
// 	lSuccess = NtAlpcSendWaitReceivePort(
// 		hConnectionPort,
// 		0,			// no flags
// 		NULL,		// SendMessage_
// 		NULL,		// SendMessageAttributes
// 		(PPORT_MESSAGE)&pmReceived,	// ReceiveBuffer
// 		&pmReceivedSize,		// BufferLength
// 		NULL,//&test,		// ReceiveMessageAttributes
// 		NULL				// no timeout
// 	);
// 	if (!NT_SUCCESS(lSuccess)) {
// 		wprintf(L"Error: 0x%X.\n", lSuccess);
// 		return 1;
// 	}
// 	else {
// 		wprintf(L"Success.\n");
// 		wprintf(L"[+] Received connection!\n\tFom Process PID: %d (Thead TID: %d)\n\tType: 0x%X.\n\tConnectionMessage Length: %d\n\n",
// 			pmReceived.PortHeader.ClientId.UniqueProcess,
// 			pmReceived.PortHeader.ClientId.UniqueThread,
// 			pmReceived.PortHeader.u2.s2.Type,
// 			pmReceived.PortHeader.u1.s1.DataLength
// 		);
// 		// Accept or Reject connection
// 		RtlSecureZeroMemory(&pmRequest, sizeof(pmRequest));
// 		pmRequest.PortHeader.MessageId = pmReceived.PortHeader.MessageId; // you need to specify the messageID in order to accept/deny the connection
// 		// Add a message
// 		LPCSTR szMessage = "Aloha Client";
// 		add_request_message(&pmRequest, szMessage);
// 
// 		/*LPVOID lpPortMessage = &(pmRequest.PortMessage);
// 		char msg[100] = "Hello Client";
// 		int lMsgLen = strlen(msg);
// 		memmove(lpPortMessage, msg, lMsgLen);
// 		pmRequest.PortHeader.u1.s1.DataLength = lMsgLen;
// 		pmRequest.PortHeader.u1.s1.TotalLength = sizeof(PORT_MESSAGE) + lMsgLen;*/
// 
// 		//pmRequest.PortHeader.u1.s1.DataLength = 0x0;
// 		//pmRequest.PortHeader.u1.s1.TotalLength = sizeof(PORT_MESSAGE) + pmRequest.PortHeader.u1.s1.DataLength;
// 		BOOL bAutoAcceptConnection = TRUE;
// 		BOOL bAcceptConnection;
// 		if (!bAutoAcceptConnection) {
// 			wprintf(L"[.] Do you want to accept the connection? [Y]: ");
// 			WCHAR wcAcceptConnection[2];
// 			std::wcin.getline(wcAcceptConnection, 2);
// 			if (wcscmp(wcAcceptConnection, L"Y") == 0 || wcscmp(wcAcceptConnection, L"y") == 0) {
// 				bAcceptConnection = TRUE;
// 			}
// 			else {
// 				bAcceptConnection = FALSE;
// 			};
// 		}
// 		else { bAcceptConnection = TRUE; }
// 
// 		SIZE_T ConnectionMessageAttributesBufferLength2;
// 		ULONG ulMsgAttr2 = ALPC_MESSAGE_ALL; // ALPC_MESSAGE_ALL;//0x70000000;//0x0E2000000; // ALPC_MESSAGE_HANDLE_ATTRIBUTE;
// 		ULONG ulAttrSize2 = AlpcGetHeaderSize(ulMsgAttr2);
// 		LPVOID lpMemALPCAttrIn2 = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ALPC_MESSAGE_ATTRIBUTES) + ulAttrSize2);
// 		PALPC_MESSAGE_ATTRIBUTES pALPCMsgAttrIn2 = (PALPC_MESSAGE_ATTRIBUTES)lpMemALPCAttrIn2;
// 		if (GetLastError() != 0) {
// 			wprintf(L"[-] Failed to allocate memory for ALPC Message attributes.");
// 			return 1;
// 		}
// 		wprintf(L"[*] Initializing ReceiveMessage Attributes (0x%X)...", ulMsgAttr2);
// 		lSuccess = AlpcInitializeMessageAttribute(
// 			ulMsgAttr2,	// attributes
// 			pALPCMsgAttrIn2,	// pointer to attributes structure
// 			ulAttrSize2,	// buffer size
// 			(PULONG)&ConnectionMessageAttributesBufferLength2
// 		);
// 		if (!NT_SUCCESS(lSuccess)) {
// 			wprintf(L"Error: 0x%X\n", lSuccess);
// 			return 1;
// 		}
// 		else wprintf(L"Success.\n");
// 
// 		CS_PORT_CONTEXT portContext;
// 		portContext.ID = 1;
// 		if (bAcceptConnection) { wprintf(L"[*] Accepting Incoming Connection...\n"); }
// 		else { wprintf(L"[*] Denying Incoming Connection...\n"); }
// 		lSuccess = NtAlpcAcceptConnectPort(
// 			&hCommunicationPort,		// Communication port handle
// 			hConnectionPort,			// Connection port handle
// 			NULL, //0x4080000,//ALPC_PORFLG_ALLOW_LPC_REQUESTS,							// no flags
// 			NULL,	// no object attributs
// 			&PortAttributes,	// no port attributes
// 			&portContext,						// no port context
// 			(PPORT_MESSAGE)&pmRequest,			// connection request
// 			pALPCMsgAttrIn2,	// no connection message attributes
// 			bAcceptConnection					// accepting the connection
// 		);
// 		if (!NT_SUCCESS(lSuccess)) {
// 			wprintf(L"Error: 0x%X\n", lSuccess);
// 			return 1;
// 		}
// 		else wprintf(L"Done. Server CommunicationPort: 0x%X\n", hCommunicationPort);
// 
// 		// Message Attributes
// 		//ALPC_MESSAGE_ATTRIBUTES SendMessageAttributes, ReceiveMessageAttributes;
// 		//ULONG ulRequiredBufferSize;
// 		//ULONG loobar;
// 		//ULONG ulALPCReqAttr = ALPC_MESSAGE_SECURITY_ATTRIBUTE | ALPC_MESSAGE_VIEW_ATTRIBUTE | ALPC_MESSAGE_CONTEXT_ATTRIBUTE | ALPC_MESSAGE_HANDLE_ATTRIBUTE | ALPC_MESSAGE_TOKEN_ATTRIBUTE;
// 		//ULONG ulAlpcAttrBuf = 0x8; // we'll start with a small buffer and if not suficient we adjust according to returned buffer
// 		//wprintf(L"[*] Initializing ReceiveMessage Attributes...");
// 		//lSuccess = AlpcInitializeMessageAttribute(
// 		//	ulALPCReqAttr,	// attributes
// 		//	&ReceiveMessageAttributes,	// pointer to attributes structure
// 		//	ulAlpcAttrBuf,	// buffer size
// 		//	&ulRequiredBufferSize
// 		//);
// 		//if ( lSuccess == STATUS_BUFFER_TOO_SMALL) {
// 		//	// Buffer size too small, try another call will returned buffer size
// 		//	lSuccess = AlpcInitializeMessageAttribute(
// 		//		ulALPCReqAttr,	// attributes
// 		//		&ReceiveMessageAttributes,	// pointer to attributes structure
// 		//		ulRequiredBufferSize,	// buffer size
// 		//		&loobar
// 		//	);
// 		//	if (!NT_SUCCESS(lSuccess)) {
// 		//		wprintf(L"Error: 0x%X\n", lSuccess);
// 		//		return 1;
// 		//	}
// 		//	else wprintf(L"Done.\n");
// 		//}
// 		//else if (!NT_SUCCESS(lSuccess)) {
// 		//	wprintf(L"Error: 0x%X\n", lSuccess);
// 		//	return 1;
// 		//}
// 		//else wprintf(L"Done.\n");
// 
// 
// 		// Send Message
// 		RtlSecureZeroMemory(&pmReceived, sizeof(pmReceived));
// 		RtlSecureZeroMemory(&pmRequest, sizeof(pmRequest));
// 		//pmReceived.PortHeader.MessageId = 0;
// 		//DWORD dwReceivedLen = pmReceived.PortHeader.u1.s1.TotalLength;
// 		// add a message
// 		LPVOID lpPortMessage2 = &(pmRequest.PortMessage);
// 		char msg2[100] = "Hello From ConnPort";
// 		int lMsgLen2 = strlen(msg2);
// 		memmove(lpPortMessage2, msg2, lMsgLen2);
// 		pmRequest.PortHeader.u1.s1.DataLength = lMsgLen2;
// 		pmRequest.PortHeader.u1.s1.TotalLength = sizeof(PORT_MESSAGE) + lMsgLen2;
// 		SIZE_T receivedLen = sizeof(pmReceived);
// 		//// Send message -- OLD WAY
// 		//DWORD nLen = 1024;
// 		//LPVOID lpHeapMem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nLen + sizeof(PORT_MESSAGE));
// 		//RtlSecureZeroMemory(&pmReceived, sizeof(pmReceived));
// 		//pmReceived.MessageId = 0;
// 		//pmReceived.u2.ZeroInit = 0;
// 		//pmReceived.ClientId.UniqueProcess = 0;
// 		//pmReceived.ClientId.UniqueThread = 0;
// 		//PPORT_MESSAGE ReceiveMessage = (PPORT_MESSAGE)lpHeapMem;
// 		// Message Attributes
// 		SIZE_T ConnectionMessageAttributesBufferLength;
// 		ULONG ulMsgAttr = ALPC_MESSAGE_ALL; // ALPC_MESSAGE_ALL;//0x70000000;//0x0E2000000; // ALPC_MESSAGE_HANDLE_ATTRIBUTE;
// 		ULONG ulAttrSize = AlpcGetHeaderSize(ulMsgAttr);
// 		LPVOID lpMemALPCAttrIn = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ALPC_MESSAGE_ATTRIBUTES) + ulAttrSize);
// 		PALPC_MESSAGE_ATTRIBUTES pALPCMsgAttrIn = (PALPC_MESSAGE_ATTRIBUTES)lpMemALPCAttrIn;
// 		if (GetLastError() != 0) {
// 			wprintf(L"[-] Failed to allocate memory for ALPC Message attributes.");
// 			return 1;
// 		}
// 		wprintf(L"[*] Initializing ReceiveMessage Attributes (0x%X)...", ulMsgAttr);
// 		lSuccess = AlpcInitializeMessageAttribute(
// 			ulMsgAttr,	// attributes
// 			pALPCMsgAttrIn,	// pointer to attributes structure
// 			ulAttrSize,	// buffer size
// 			(PULONG)&ConnectionMessageAttributesBufferLength
// 		);
// 		if (!NT_SUCCESS(lSuccess)) {
// 			wprintf(L"Error: 0x%X\n", lSuccess);
// 			return 1;
// 		}
// 		else wprintf(L"Done.\n");
// 		// Receive Message
// 		wprintf(L"[*] Waiting for incoming data on port '0x%X'...", hCommunicationPort);
// 		lSuccess = NtAlpcSendWaitReceivePort(
// 			hConnectionPort,		// we need to use the connection port
// 			NULL,			// no flags
// 			NULL, //(PPORT_MESSAGE)&pmRequest,		// SendMessage_
// 			NULL,		// SendMessageAttributes
// 			(PPORT_MESSAGE)&pmReceived,//ReceiveMessage,  // ReceiveBuffer
// 			(PULONG)&receivedLen,		// Receive BufferLength
// 			pALPCMsgAttrIn,		// ReceiveMessageAttributes
// 			NULL		// no timeout
// 		);
// 		if (!NT_SUCCESS(lSuccess)) {
// 			wprintf(L"[-] Error: 0x%X\n", lSuccess);
// 			return 1;
// 		}
// 		else wprintf(L"Done.\n");
// 		wprintf(L"[*] Trying to impersonate client. Valid Message Attributes: 0x%X ...\n", pALPCMsgAttrIn->ValidAttributes);
// 		// Get CONTEXT ATTR
// 		ALPC_MESSAGE_ATTRIBUTES inMsgAttr = *pALPCMsgAttrIn;
// 		if (true) {
// 			LPVOID lpMessageAttr = AlpcGetMessageAttribute(pALPCMsgAttrIn, ALPC_MESSAGE_TOKEN_ATTRIBUTE);
// 			ALPC_TOKEN_ATTR tokenATTR = *(PALPC_TOKEN_ATTR)lpMessageAttr;
// 
// 			LPVOID lpMessageAttr2 = AlpcGetMessageAttribute(pALPCMsgAttrIn, ALPC_MESSAGE_SECURITY_ATTRIBUTE);
// 			ALPC_SECURITY_ATTR secATTR = *(PALPC_SECURITY_ATTR)lpMessageAttr2;
// 			//HANDLE hToken = tokenATTR.TokenHandle;
// 			//ExtractTokenInformation(hToken);
// 			//LPWSTR stokenSID = {0};
// 			//BOOL boola = ConvertSidToStringSid(tokenATTR.TokenSid, &stokenSID);
// 			//DWORD err = GetLastError();
// 			//wprintf(L"TokenSID: %s", stokenSID);
// 			//int a = 4;
// 		}
// 		//lSuccess = impersonate_client(hConnectionPort, pmReceived);
// 		//if (!NT_SUCCESS(lSuccess)) {
// 		//	wprintf(L"[-] Error: 0x%X\n", lSuccess);
// 		//}
// 		//else wprintf(L"Done.\n");
// 		lSuccess = impersonate_client(hCommunicationPort, pmReceived);
// 		if (!NT_SUCCESS(lSuccess)) {
// 			wprintf(L"[-] Error: 0x%X\n", lSuccess);
// 		}
// 		else wprintf(L"Done.\n");
// 
// 		// Trying to impersonate
// 		//wprintf(L"Trying to impersonate Client...\n");
// 		//lSuccess = impersonate_client(hConnectionPort, ReceiveMessage);
// 		//if (!NT_SUCCESS(lSuccess)) {
// 		//	wprintf(L"[-] Error: 0x%X\n", lSuccess);
// 		//	return 1;
// 		//}
// 		//else wprintf(L"Done.\n");
// 
// 		//wprintf(L"[*] Getting Message attribtues...");
// 		//LPVOID foobar = AlpcGetMessageAttribute(&ReceiveMessageAttributes, ALPC_MESSAGE_CONTEXT_ATTRIBUTE); //0x08000000
// 		//ALPC_CONTEXT_ATTR boobar;
// 		//if (ReceiveMessageAttributes.ValidAttributes == ALPC_MESSAGE_CONTEXT_ATTRIBUTE) {
// 		//	wprintf(L"Success.\n");
// 		//	boobar = *(PALPC_CONTEXT_ATTR)foobar;
// 		//}
// 		//else {
// 		//	wprintf(L"Failed (0x%X).\n", ReceiveMessageAttributes.ValidAttributes);
// 		//}
// 
// 		//pmReceived = *(PORT_MESSAGE*)lpHeapMem;
// 		wprintf(L"[*] Received Data of Length: %d\n[+]\t", pmReceived.PortHeader.u1.s1.DataLength);
// 		for (int i = 0; i <= pmReceived.PortHeader.u1.s1.DataLength; i++) {
// 			BYTE* pbyReadPointer = (BYTE*)pmReceived.PortMessage + i;//(BYTE*)((BYTE*)lpHeapMem + i + sizeof(PORT_MESSAGE));
// 			BYTE byRead = *pbyReadPointer;
// 			printf("%c", byRead);
// 		}
// 	}
// 	//if (status != ERROR_SUCCESS) wprintf(L"Error: %d", GetLastError());
// 
// 	return 0;
// }


