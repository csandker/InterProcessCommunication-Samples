//#include <winnt.h>
//#include <sddl.h>

#include "..\CPP-Util\ALPC.h"
#include "..\CPP-Util\CommonALPC.cpp"

const long MAX_MESSAGE_SIZE = 0x100;

int main() {
	HANDLE hSrvCommPort;
	NTSTATUS lSuccess;
	CLIENT_ID clientID;
	ALPC_MESSAGE pmReceived;
	ALPC_MESSAGE pmSend;
	SIZE_T ulSendSize;
	SIZE_T ulReceivedSize;
	SECURITY_QUALITY_OF_SERVICE SecurityQos;
	ALPC_PORT_ATTRIBUTES PortAttributes;
	PALPC_MESSAGE_ATTRIBUTES pMsgAttrSend;
	PALPC_MESSAGE_ATTRIBUTES pMsgAttrReceived;
	ALPC_MESSAGE_ATTRIBUTES inMsgAttr;
	UNICODE_STRING usPortName;
	LPCWSTR pwsPortName = L"\\RPC Control\\CSALPCPort"; // the name of the ALPC port we're attempting to connect to 
	PSID pSID = NULL;
	BOOL bVerifyServerSID = FALSE;

	//
	// INITIALIZAITONS
	//
	DWORD dwPID = GetCurrentProcessId();
	DWORD dwTID = GetCurrentThreadId();
	clientID.UniqueProcess = &dwPID;
	clientID.UniqueThread = &dwTID;
	// Received Message attributes.. I'm fine with getting all of them
	pMsgAttrReceived = alloc_message_attribute(ALPC_MESSAGE_ATTRIBUTE_ALL);

	// QOS
	SecurityQos.ImpersonationLevel = SecurityImpersonation; // SecurityIdentification; // ;
	SecurityQos.ContextTrackingMode = SECURITY_STATIC_TRACKING;
	SecurityQos.EffectiveOnly = 0;
	SecurityQos.Length = sizeof(SecurityQos);
	// ALPC Port Attributs
	PortAttributes.Flags = ALPC_PORTFLG_ALLOW_DUP_OBJECT | ALPC_PORTFLG_ALLOWIMPERSONATION | ALPC_PORTFLG_LRPC_WAKE_POLICY1 | ALPC_PORTFLG_LRPC_WAKE_POLICY2 | ALPC_PORTFLG_LRPC_WAKE_POLICY3; // | ALPC_PORTFLG_DIRECT_MESSAGE;
	PortAttributes.MaxMessageLength = sizeof(ALPC_MESSAGE); // technically the hard limit for this is 65535, if no constrains you can use AlpcMaxAllowedMessageLength() to set this limit
	PortAttributes.MemoryBandwidth = 512;
	PortAttributes.MaxSectionSize = 0xffffffff; // 20000; 
	PortAttributes.MaxViewSize = 0xffffffff; // 20000; // sizeof(PORT_VIEW); 
	PortAttributes.MaxTotalSectionSize = 0xffffffff;// 20000;
	PortAttributes.DupObjectTypes = 0xffffffff;
	PortAttributes.MaxPoolUsage = 0xffffffff; // 0x4000;
	PortAttributes.SecurityQos = SecurityQos;

	wprintf(L"[*] Starting Client. PID: %d | TID: %d\n", dwPID, dwTID);

	RtlInitUnicodeString(&usPortName, pwsPortName);	// Initialize Unicode String
	//
	// CONNECT TO SERVER PORT
	//
	RtlSecureZeroMemory(&pmSend, sizeof(pmSend));
	add_request_message(&pmSend, "Hello Server");
	ulSendSize = sizeof(pmSend);
	// OPTIONALLY: Verify SID 
	if (bVerifyServerSID) {
		ConvertStringSidToSid(L"S-1-5-21-258271829-1673813934-3670761721-1000", &pSID);
	}
	wprintf(L"[*] Connecting to port '%s'...", pwsPortName);
	
	/*LARGE_INTEGER timeout;
	timeout.QuadPart = 10000000;*/
	lSuccess = NtAlpcConnectPort(
		&hSrvCommPort,				// REQUIRED: empty Communication port handle, fill be set by kernel
		&usPortName,				// REQUIRED: Server Connect port name to connect to
		NULL,						// OPTIONAL: Object Attributes, none in this case
		&PortAttributes,			// OPTIONAL: PortAtrributes, used to set various port connection attributes, most imporatnly port flags
		ALPC_SYNC_CONNECTION,		// OPTOONAL: Message Flags, no Flags
		pSID,						// OPTIONAL: Server SID
		(PPORT_MESSAGE)&pmSend,		// connection message
		&ulSendSize,				// connection message size
		NULL,//pMsgAttrSend,		// out messages attribtus
		pMsgAttrReceived,			// in message attributes
		0 //&timeout				// OPTIONAL: Timeout, none in this case
	);
	if (lSuccess == STATUS_PORT_CONNECTION_REFUSED) {
		wprintf(L"\n[*] The Server denied the connection request.\n");
		return 0;
	}
	else if (lSuccess == STATUS_SERVER_SID_MISMATCH) {
		wprintf(L"\n[*] The specified Server SID does not match with the Server. Connected to the wrong server?.\n");
		return 0;
	}
	else if (!NT_SUCCESS(lSuccess)) {
		wprintf(L"Error: 0x%X\n", lSuccess);
		return 1;
	}
	else wprintf(L"Success.\n");
	print_received_alpc_message(pmSend, NULL, false);
	print_valid_message_attribues(pMsgAttrReceived);

	// Impersonation Attempt
	/*lSuccess = impersonate_client(hSrvCommPort, pmSend);
	if (!NT_SUCCESS(lSuccess)) {
		wprintf(L"[-] Failed to impersonate: 0x%X\n", lSuccess);
	};*/

	//
	// Send Message To Server
	//
	RtlSecureZeroMemory(&pmReceived, sizeof(pmReceived));
	RtlSecureZeroMemory(&pmSend, sizeof(pmSend));
	add_request_message(&pmSend, "Whatzzzzzuuuuuup");

	HANDLE hServerSection;
	SIZE_T nServerSectionSize;
	wprintf(L"[*] Creating Port Section...");
	lSuccess = NtAlpcCreatePortSection(
		hSrvCommPort,		// _In_ HANDLE PortHandle,
		0,					// _In_ flags	// 0x40000 found in rpcrt4.dll
		NULL,				// OPTIONAL SectionHandle,
		1024,				// SectionSize,
		&hServerSection,	// _Out_ HANDLE AlpcSectionHandle,
		&nServerSectionSize // _Out_ ActualSectionSize
	);
	if (!NT_SUCCESS(lSuccess)) {
		wprintf(L"Error: 0x%X\n", lSuccess);
		return 1;
	}
	else wprintf(L"Success, Section created. Size: %d\n", nServerSectionSize);
	pMsgAttrSend = setup_sample_message_attributes(hSrvCommPort, hServerSection, ALPC_MESSAGE_VIEW_ATTRIBUTE); //ALPC_MESSAGE_SECURITY_ATTRIBUTE

	wprintf(L"[*] [PID: %d; TID: %d] Sending Message (len: %d) to Port ....", GetCurrentProcessId(), GetCurrentThreadId(), pmSend.PortHeader.u1.s1.DataLength);
	pMsgAttrSend->ValidAttributes |= ALPC_MESSAGE_VIEW_ATTRIBUTE;  // Mark an attribute as valid
	lSuccess = NtAlpcSendWaitReceivePort(
		hSrvCommPort,				// our port handle
		ALPC_MSGFLG_SYNC_REQUEST,		// message Flags
		(PPORT_MESSAGE)&pmSend,		// sending message
		pMsgAttrSend,				// sending attributes
		(PPORT_MESSAGE)&pmReceived,		// receiving message
		&ulReceivedSize,						// receiving message length
		pMsgAttrReceived,			// receive message attributes
		0							// no timeout
	);
	if (!NT_SUCCESS(lSuccess)) {
		wprintf(L"Error: 0x%X\n", lSuccess);
		return 1;
	}
	else wprintf(L"Success.\n");
	print_received_alpc_message(pmReceived, NULL, false);
	print_valid_message_attribues(pMsgAttrReceived);
			
	//
	// CLOSING Port
	//
	wprintf(L"[*] Closing Port connection...");
	NtAlpcDisconnectPort(hSrvCommPort, 0);
	if (!NT_SUCCESS(lSuccess)) {
		wprintf(L"Error: 0x%X\n", lSuccess);
		return 1;
	}
	else wprintf(L"Success.\n");
	CloseHandle(hSrvCommPort);

	// Finish
	return 0;
}