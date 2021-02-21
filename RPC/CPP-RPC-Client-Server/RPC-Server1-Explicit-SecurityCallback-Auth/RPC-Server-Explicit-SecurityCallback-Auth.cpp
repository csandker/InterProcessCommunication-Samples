
#include <iostream>
#include <windows.h>
#include <sddl.h>
#include <ntdsapi.h>
#include <aclapi.h> // For Window Security
#include "..\RPC-Interface1\Interface1-Explicit_h.h"

#include "..\RPC-Util\Command.cpp"

// global param
int g_bImpersonateClient = TRUE;

// Naive security callback.
RPC_STATUS CALLBACK SecurityCallback(RPC_IF_HANDLE hInterface, void* pBindingHandle)
{
	wprintf(L"[*] Inside Security Callback\n");
	return RPC_S_OK; // Always allow anyone.
}

// Future server function.
int Output(
	/* [in] */ handle_t hBinding,
	/* [string][in] */ const char* pszOutput)
{
	BOOL bSuccess;
	printf("[~] Client Message: %s\n", pszOutput);
	if (g_bImpersonateClient) {
		// Try to impersonate client
		wprintf(L"[*] Trying to impersonate client...");
		RPC_STATUS status = RpcImpersonateClient(hBinding);
		if (status != RPC_S_OK) {
			printf("failed (Error: %d)\n", status);
			return 10; // arbitrary chosen, this function could return void or anything else. This is just a PoC
		}
		else {
			printf("Success!\n");
			// Execute Window command (cause cmd.exe will spawn a new GUI window)
			wchar_t command[] = L"C:\\Windows\\System32\\cmd.exe";
			bSuccess = execImpersonatedWindowCommand(hBinding, command);
		}
	}
	// -- Return value, arbitrary chosen
	return 5;
}


void Shutdown(
	/* [in] */ handle_t hBinding)
{
	wprintf(L"[*] Calling: RpcMgmtStopServerListening ...\n");
	RPC_STATUS status;
	status = RpcMgmtStopServerListening(NULL); // NULL indicates that we want to stop our own server, instead of a remote server

	if (status)
		exit(status);

	wprintf(L"[*] Calling: RpcServerUnregisterIf...\n");
	status = RpcServerUnregisterIf(
		NULL, // NULL indicates that we want to unregister all interfaces
		NULL, // NULL indicates that we want to remove interfaces for all UUIDs previously specfied with our s_ifspec
		FALSE // FALSE indicates that we want to remove the interface from the registry immediatly
	);

	if (status)
		exit(status);
}

int main()
{
	wprintf(L"[*] Starting RPC Server!\n");
	handle_t hBinding = NULL;
	BOOL bRpcEpRegister = TRUE;
	// SPN vars
	DWORD spnStatus;
	DWORD serverSPNLen = MAX_PATH;
	WCHAR wszSpn[MAX_PATH] = { 0 };
	DWORD cSPN;
	TCHAR** ppFoundSPNs;
	// RPC Server vars
	RPC_STATUS rpcStatus;
	RPC_WSTR pszSpn;
	RPC_WSTR pszProtSeq;
	RPC_WSTR pszTCPPort;
	CSENDPOINTTYPE pwsEndpointType = NamedPipe;
	switch (pwsEndpointType)
	{
	case TCP_IP:
		pszProtSeq = (RPC_WSTR)L"ncacn_ip_tcp"; //reinterpret_cast<RPC_WSTR>(L"ncacn_ip_tcp");
		pszTCPPort = (RPC_WSTR)L"8989"; //reinterpret_cast<RPC_WSTR>(L"8989");
		break;
	case NamedPipe:
		pszProtSeq = (RPC_WSTR)L"ncacn_np";
		pszTCPPort = (RPC_WSTR)L"\\pipe\\FRPC-NP";
		break;
	case LRPC:
		pszProtSeq = (RPC_WSTR)L"ncalrpc";
		pszTCPPort = (RPC_WSTR)L"\\RPC Control\\FRPC-LRPC";
		break;
	default:
		pszProtSeq = (RPC_WSTR)L"";
		pszTCPPort = (RPC_WSTR)L"";
		break;
	}

	/*
	// SET SPN programatically
	//
	Output(hBinding, "Registering SPN...");
	spnStatus = DsMakeSpn(
		L"host",			// service class, could be any string
		L"Spaceland-SRV-1",		// service name, could be any string
		L"Spaceland-SRV-1.MonkeyIsland.local",		// instance name that specifies the IP or DNS name
		0,					// instance port, 0 indicates the use of the default port
		NULL,				// referrer, not used here
		&serverSPNLen,		//
		wszSpn				// empty string
	);
	*/

	// GET already registered SPN (instead of setting one)
	// (each domain computer comes with a few default SPNs, one of which is the "Host" SPN)
	//
	wprintf(L"[*] Getting SPN...\n");
	spnStatus = DsGetSpn(
		DS_SPN_NB_HOST,	// SPN format ServiceClass/ InstanceName: InstancePort
		L"Host",		// SPN class
		NULL,			// DNS name of SPN, not required therefor NULL
		0,				// SPN instance port, 0 to specfiy that SPN does not include port
		0,				// number additional instance names
		NULL,			// no additional instance names
		NULL,			// no additional instance ports
		&cSPN,			// pointer to DWORD containing the number of SPNs found
		&ppFoundSPNs	// Pointer to a variable that receives a pointer to an array of SPNs
	);

	if (spnStatus != RPC_S_OK) {
		wprintf(L"[-] Failed to get or set SPN\n");
		return 1;
	}

	pszSpn = (RPC_WSTR)ppFoundSPNs;
	wprintf(L"[+] Found SPN: %s \n", *ppFoundSPNs);
	// Free the Array of found SPNs
	DsFreeSpnArray(cSPN, ppFoundSPNs);

	// Create Binding information
	wprintf(L"[*] Creating Binding Information using protocol '%s' at '%s'.\n", pszProtSeq, pszTCPPort);
	rpcStatus = RpcServerUseProtseqEp(
		pszProtSeq,						// Use TCP/IP protocol.
		RPC_C_PROTSEQ_MAX_REQS_DEFAULT, // Backlog queue length for TCP/IP.
		pszTCPPort,						// TCP/IP port to use.
		NULL							// No Secuirty Descriptor
	);

	if (rpcStatus != RPC_S_OK) {
		wprintf(L"[-] RpcServerUseProtseqEp() failed with status: %d.\n", rpcStatus);
		exit(rpcStatus);
	}

	// Registers the Example1 interface.
	wprintf(L"[*] Registering Server interface:.\n");
	rpcStatus = RpcServerRegisterIf2(
		Example1_v1_0_s_ifspec,              // Interface to register.
		NULL,								 // Nil-type UUID
		NULL,                                // Use the MIDL generated entry-point vector.
		RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH, // Forces use of security callback.
		RPC_C_LISTEN_MAX_CALLS_DEFAULT,      // Use default number of concurrent calls.
		(unsigned)-1,                        // Infinite max size of incoming data blocks.
		SecurityCallback);                   // Naive security callback.

	if (rpcStatus != RPC_S_OK) {
		wprintf(L"[-] RpcServerRegisterIf2() failed with status: %d.\n", rpcStatus);
		exit(rpcStatus);
	}

	// Register authentication info
	wprintf(L"[*] Register Authentication Information.\n");
	rpcStatus = RpcServerRegisterAuthInfo(
		pszSpn,					// Server principal name
		RPC_C_AUTHN_WINNT,		// using NTLM as authentication service provider as defined at https://docs.microsoft.com/en-us/windows/win32/rpc/authentication-service-constants
		NULL,					// Use default key function, is ignored for NTLM SSP as per https://docs.microsoft.com/en-us/windows/win32/api/rpcdce/nf-rpcdce-rpcserverregisterauthinfo
		NULL);					// No arg for key function

	if (rpcStatus != RPC_S_OK) {
		wprintf(L"[-] RpcServerRegisterIf2() failed with status: %d.\n", rpcStatus);
		exit(rpcStatus);
	}
 
	if ( bRpcEpRegister ) {
		// This server uses Well-known endpoints
		// Therefore the server doesn't has to report its interfaces to the RPC Endpoint Mapper in
		// Order for the clients to be able to connect
		// We can do this nevertheless just so that we can find this server when quering the Endpoint Mapper
		wprintf(L"[*] RpcServerInqBindings...\n");
		RPC_BINDING_VECTOR* pbindingVector = 0;
		rpcStatus = RpcServerInqBindings(&pbindingVector);
		if (rpcStatus != RPC_S_OK) {
			wprintf(L"[-] RpcServerInqBindings() failed with status: %d.\n", rpcStatus);
			exit(rpcStatus);
		}
		wprintf(L"[*] RpcEpRegister...\n");
		rpcStatus = RpcEpRegister(
			Example1_v1_0_s_ifspec,
			pbindingVector,
			0,
			(RPC_WSTR)L"TEST?"
		);
		if (rpcStatus != RPC_S_OK) {
			wprintf(L"[-] RpcEpRegister() failed with status: %d.\n", rpcStatus);
			exit(rpcStatus);
		}
	}
	

	// Start to listen for remote procedure
	// calls for all registered interfaces.
	// This call will not return until
	// RpcMgmtStopServerListening is called.
	wprintf(L"[*] Listening for client connection.\n");
	rpcStatus = RpcServerListen(
		1,                                   // Recommended minimum number of threads.
		RPC_C_LISTEN_MAX_CALLS_DEFAULT,      // Recommended maximum number of threads.
		FALSE);                              // Start listening now.

	if (rpcStatus != RPC_S_OK) {
		wprintf(L"[-] RpcServerListen() failed with status: %d.\n", rpcStatus);
		exit(rpcStatus);
	}

	wprintf(L"[*] RPC Server stopped, exiting now...\n");
}

/******************************************************/
/*         MIDL allocate and free                     */
/*		   The runtime uses these two functions for	  */
/*		   allocating / deallocating				  */
/******************************************************/

void __RPC_FAR * __RPC_USER midl_user_allocate(size_t len)
{
	return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR * ptr)
{
	free(ptr);
}