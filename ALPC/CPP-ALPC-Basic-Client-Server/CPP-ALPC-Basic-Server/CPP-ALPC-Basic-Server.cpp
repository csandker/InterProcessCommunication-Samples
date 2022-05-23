#include <iostream>

#include "..\CPP-Util\ALPC.h"
#include "..\CPP-Util\CommonALPC.cpp"


int main(){
	//
	// DEFINITIONS
	//
	NTSTATUS lSuccess;
	HANDLE hConnectionPort;
	HANDLE hCommunicationPort;
	UNICODE_STRING usAlpcPortName;
	OBJECT_ATTRIBUTES objAttributes;
	ALPC_PORT_ATTRIBUTES PortAttributes;
	SECURITY_QUALITY_OF_SERVICE SecurityQos;
	ALPC_MESSAGE pmReceived;
	ALPC_MESSAGE pmRequest;
	SIZE_T ulReceivedSize;
	PALPC_MESSAGE_ATTRIBUTES pMsgAttrSend;
	PALPC_MESSAGE_ATTRIBUTES pMsgAttrReceived;
	LPCWSTR pwsPortName = L"\\RPC Control\\CSALPCPort"; // the name of the ALPC port we're creating
	CS_PORT_CONTEXT portContext;
	UUID uuid;
	BOOL bAttemptImpersonation = TRUE;
	//
	// INITIALIZAITONS
	//
	DWORD dwPID = GetCurrentProcessId();
	DWORD dwTID = GetCurrentThreadId();
	// QOS
	//RtlZeroMemory(&SecurityQos, sizeof(SECURITY_QUALITY_OF_SERVICE));
	SecurityQos.ImpersonationLevel = SecurityIdentification; // SecurityImpersonation; // ; // ; // ;// ;
	SecurityQos.ContextTrackingMode = SECURITY_STATIC_TRACKING;
	SecurityQos.EffectiveOnly = 0;
	SecurityQos.Length = sizeof(SecurityQos);
	// ALPC Port Attributs
	PortAttributes.Flags = ALPC_PORTFLG_ALLOW_DUP_OBJECT | ALPC_PORTFLG_ALLOWIMPERSONATION | ALPC_PORTFLG_LRPC_WAKE_POLICY1 | ALPC_PORTFLG_LRPC_WAKE_POLICY2 | ALPC_PORTFLG_LRPC_WAKE_POLICY3; //0xb84a3f0;// ALPC_PORTFLG_ALLOW_DUP_OBJECT | ALPC_PORTFLG_AllowImpersonation | ALPC_PORTFLG_LRPC_WAKE_POLICY1 | ALPC_PORTFLG_LRPC_WAKE_POLICY2 | ALPC_PORTFLG_LRPC_WAKE_POLICY3; // ; //0x8080000;// ALPC_PORFLG_ALLOW_LPC_REQUESTS;// ALPC_PORFLG_ALLOW_LPC_REQUESTS;// | ALPC_PORFLG_SYSTEM_PROCESS;//0x010000 | 0x020000;	// Found '0x3080000' in rpcrt4.dll
	PortAttributes.MaxMessageLength = sizeof(ALPC_MESSAGE); // technically the hard limit for this is 65535, if no constrains you can use AlpcMaxAllowedMessageLength() to set this limit
	PortAttributes.MemoryBandwidth = 512;
	PortAttributes.MaxPoolUsage = 0xffffffff;
	PortAttributes.MaxSectionSize = 0xffffffff; // 20000;  
	PortAttributes.MaxViewSize = 0xffffffff; // 20000; // sizeof(PORT_VIEW); 
	PortAttributes.MaxTotalSectionSize = 0xffffffff; // 20000;
	PortAttributes.DupObjectTypes = 0xffffffff;
	RtlSecureZeroMemory(&SecurityQos, sizeof(SecurityQos));
	PortAttributes.SecurityQos = SecurityQos;
	// Security Descriptopr
	LPCWSTR szDACL = L"D:(A;OICI;GAGW;;;AU)";// Allow full control to authenticated users
	PSECURITY_DESCRIPTOR pSD = create_sd_from_string(szDACL);
	
	// Received Message attributes.. I'm fine with getting all of them
	pMsgAttrReceived = alloc_message_attribute(ALPC_MESSAGE_ATTRIBUTE_ALL);

	wprintf(L"[*] Starting Server. PID: %d | TID: %d\n", dwPID, dwTID);
	
	// 
	// CREATE PORT
	//
	RtlInitUnicodeString(&usAlpcPortName, pwsPortName);	// Initialize Unicode String
	InitializeObjectAttributes(						// set up OBJECT_ATTRIBUTES structure
		&objAttributes,		// the pointer to the OBJECT_ATTRIBUTES structure
		&usAlpcPortName,	// the name the object we want to obtain a handle for
		0,					// no flags, ref.: https://docs.microsoft.com/en-us/windows/win32/api/ntdef/nf-ntdef-initializeobjectattributes
		NULL,				// no root directry handle as our port name is a full qualified path
		NULL//pSD				// security descriptor
	);
	wprintf(L"[*] Creating ALPC Port '%s'...", pwsPortName);
	lSuccess = NtAlpcCreatePort(
		&hConnectionPort,			// the handle to our port name
		&objAttributes,		// the OBJECT_ATTRIBUTES structure we just initialized
		&PortAttributes //0 				// additional port attributes
	);
	if (!NT_SUCCESS(lSuccess)) {
		wprintf(L"Error: 0x%X\n", lSuccess);
		return 1;
	}
	else wprintf(L"Success.\n");
	print_port_flags(PortAttributes.Flags);

	//
	// Create Section - just to play around with sections, not necessarily needed
	//
	HANDLE hServerSection;
	SIZE_T nServerSectionSize;
	wprintf(L"[*] Creating Port Section...");
	lSuccess = NtAlpcCreatePortSection(
		hConnectionPort, //_In_ HANDLE PortHandle,
		0, //_In_ ULONG Flags,	// 0x40000 found in rpcrt4.dll
		NULL, //_In_opt_ HANDLE SectionHandle,
		1000, // _In_ SIZE_T SectionSize,
		&hServerSection, //_Out_ HANDLE AlpcSectionHandle,
		&nServerSectionSize //_Out_ PSIZE_T ActualSectionSize
	);
	if (!NT_SUCCESS(lSuccess)) {
		wprintf(L"Error: 0x%X\n", lSuccess);
		return 1;
	}
	else wprintf(L"Success, Section created. Size: %d\n", nServerSectionSize);
	
	//
	// Setup Message attributes
	//	--> We set these up once, and reuse the buffer while specifing different valid attributes for different messages
	//
	pMsgAttrSend = setup_sample_message_attributes(hConnectionPort, hServerSection, ALPC_MESSAGE_SECURITY_ATTRIBUTE | ALPC_MESSAGE_VIEW_ATTRIBUTE | ALPC_MESSAGE_HANDLE_ATTRIBUTE);
	
	//
	// WAIT FOR INITIAL CONNECTION
	//
	wprintf(L"[*] Wait for incoming connections...");
	RtlSecureZeroMemory(&pmReceived, sizeof(pmReceived));
	lSuccess = NtAlpcSendWaitReceivePort(
		hConnectionPort,
		ALPC_MSGFLG_NONE,			// no flags
		NULL,		// SendMessage_
		NULL,		// SendMessageAttributes
		(PPORT_MESSAGE)&pmReceived,	// ReceiveBuffer
		&ulReceivedSize,		// BufferLength
		pMsgAttrReceived,//&test,		// ReceiveMessageAttributes
		0				// no timeout
	);
	if (!NT_SUCCESS(lSuccess)) {
		wprintf(L"Error: 0x%X.\n", lSuccess);
		return 1;
	}
	else {
		wprintf(L"Success.\n");
	}
	print_received_alpc_message(pmReceived, NULL, false);
	print_valid_message_attribues(pMsgAttrReceived, TRUE);

	
	//
	// ACCEPT OR REJECT MESSAGE
	//
	BOOL bAcceptConnection = accept_connection_request();
	RtlSecureZeroMemory(&pmRequest, sizeof(pmRequest));
	pmRequest.PortHeader.MessageId = pmReceived.PortHeader.MessageId; // Set the messageID from the received client connection to send the message back to the same client
	
	// Create client context so I can identify the client in every call
	portContext.PID = (ULONG)pmReceived.PortHeader.ClientId.UniqueProcess;
	portContext.TID = (ULONG)pmReceived.PortHeader.ClientId.UniqueThread;
	portContext.ID = portContext.PID + portContext.TID; // not a good UUID for production purposes...just as an example, could be anything
	
	pMsgAttrSend->ValidAttributes |= ALPC_MESSAGE_SECURITY_ATTRIBUTE; // ALPC_MESSAGE_HANDLE_ATTRIBUTE; // Mark the an attribute as valid
	if (bAcceptConnection) { 
		wprintf(L"[*] Accepting Incoming Connection...");
		add_request_message(&pmRequest, "I'll accept your connection...");
	}
	else {
		wprintf(L"[*] Denying Incoming Connection...");
		add_request_message(&pmRequest, "Nope, you're not allowed to connect...");
	}
	lSuccess = NtAlpcAcceptConnectPort(
		&hCommunicationPort,		// Communication port handle
		hConnectionPort,			// Connection port handle
		ALPC_SYNC_CONNECTION,		// connection flags
		NULL,						// no object attributs
		&PortAttributes, //0,		// port attributes
		&portContext,				// port context
		(PPORT_MESSAGE)&pmRequest,	// connection request
		pMsgAttrSend,				// connection message attributes
		bAcceptConnection			// accepting the connection
	);
	if (!NT_SUCCESS(lSuccess)) {
		wprintf(L"Error: 0x%X\n", lSuccess);
		return 1;
	}
	else wprintf(L"Done\n");

	//
	// WAIT FOR INCOMING DATA
	//
	RtlSecureZeroMemory(&pmReceived, sizeof(pmReceived));
	RtlSecureZeroMemory(&pmRequest, sizeof(pmRequest));
	wprintf(L"[*] Waiting for incoming data...");
	lSuccess = NtAlpcSendWaitReceivePort(
		hConnectionPort,					// we need to use the connection port
		ALPC_MSGFLG_LPC_MODE,				// message flags
		NULL,//(PPORT_MESSAGE)&pmRequest,	//Sending message
		NULL, //pMsgAttrSend,				// SendMessageAttributes
		(PPORT_MESSAGE)&pmReceived,			// ReceivingMessage
		&ulReceivedSize,					// Receive BufferLength
		pMsgAttrReceived,					// ReceiveMessageAttributes
		0									// no timeout
	);
	if (!NT_SUCCESS(lSuccess)) {
		wprintf(L"[-] Error: 0x%X\n", lSuccess);
		return 1;
	}
	else wprintf(L"Success.\n");
	print_received_alpc_message(pmReceived, hConnectionPort, true);
	print_valid_message_attribues(pMsgAttrReceived, TRUE);

	//
	// ATTEMPT IMPERSONATION 
	//
	if (bAttemptImpersonation) {
		wprintf(L"[*] Trying to impersonate client...\n");
		lSuccess = impersonate_client(hCommunicationPort, pmReceived);
		if (!NT_SUCCESS(lSuccess)) {
			wprintf(L"[-] Error: 0x%X\n", lSuccess);
		}
		else wprintf(L"Done.\n");
	}

	//
	// REPLY TO client 
	//	-- note this could as well have been included in the previous NtAlpcSendWaitReceivePort call, this is just to show case that you could also do something with the message before responding
	//
	add_request_message(&pmRequest, "Data received!");
	pmRequest.PortHeader.MessageId = pmReceived.PortHeader.MessageId;
	pMsgAttrSend->ValidAttributes |= ALPC_MESSAGE_VIEW_ATTRIBUTE; // ALPC_MESSAGE_SECURITY_ATTRIBUTE; // Mark the an attribute as valid
	wprintf(L"[*] Sending data to port");
	lSuccess = NtAlpcSendWaitReceivePort(
		hConnectionPort,		// we need to use the connection port
		ALPC_MSGFLG_REPLY_MESSAGE,			// no flags
		(PPORT_MESSAGE)&pmRequest, //		// SendMessage_
		pMsgAttrSend,		// SendMessageAttributes
		NULL, //(PPORT_MESSAGE)&pmReceived,//ReceiveMessage,  // ReceiveBuffer
		NULL, //&pmReceivedSize,		// Receive BufferLength
		NULL, // pMsgAttrReceived,		// ReceiveMessageAttributes
		0		// no timeout
	);
	if (!NT_SUCCESS(lSuccess)) {
		wprintf(L"[-] Error: 0x%X\n", lSuccess);
		return 1;
	}
	else wprintf(L"Success.\n");

	// We could wait for new data, but this is just a PoC...
	//wprintf(L"Sleeping for 1 seconds..");
	//Sleep(1000);
	//while (TRUE) {

	//	wprintf(L"[*] Wait for incoming connections...");
	//	RtlSecureZeroMemory(&pmReceived, sizeof(pmReceived));
	//	lSuccess = NtAlpcSendWaitReceivePort(
	//		hConnectionPort,
	//		ALPC_MSGFLG_LPC_MODE,			// no flags
	//		NULL,		// SendMessage_
	//		NULL,		// SendMessageAttributes
	//		(PPORT_MESSAGE)&pmReceived,	// ReceiveBuffer
	//		&ulReceivedSize,		// BufferLength
	//		pMsgAttrReceived,//&test,		// ReceiveMessageAttributes
	//		0				// no timeout
	//	);
	//	if (!NT_SUCCESS(lSuccess)) {
	//		wprintf(L"Error: 0x%X.\n", lSuccess);
	//		return 1;
	//	}
	//	else {
	//		wprintf(L"Success.\n");
	//	}
	//	print_received_alpc_message(pmReceived, hConnectionPort, FALSE);
	//	print_valid_message_attribues(pMsgAttrReceived, TRUE);
	//}

	//
	// CLOSING Port
	//
	wprintf(L"[*] Closing Port connection...");
	NtAlpcDisconnectPort(hCommunicationPort, 0);
	NtAlpcDisconnectPort(hConnectionPort, 0);
	if (!NT_SUCCESS(lSuccess)) {
		wprintf(L"Error: 0x%X\n", lSuccess);
	}
	else wprintf(L"Success.\n");
	CloseHandle(hConnectionPort);
	CloseHandle(hCommunicationPort);

	// Finish
	return 0;
}