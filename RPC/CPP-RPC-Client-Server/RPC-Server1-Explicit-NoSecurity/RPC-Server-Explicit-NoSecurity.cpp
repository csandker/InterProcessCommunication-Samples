
#include <iostream>
#include <Windows.h>
#include <ntdsapi.h> // For SPN APIs
#include "..\RPC-Interface1\Interface1-Explicit_h.h"

// Future server function.
int Output(
	/* [in] */ handle_t hBinding,
	/* [string][in] */ const char* pszOutput)
{
	std::cout << pszOutput << std::endl;
	//szReturn = "Your Output was: ";
	return 5;
}


void Shutdown(
	/* [in] */ handle_t hBinding)
{
	Output(hBinding, "Calling: RpcMgmtStopServerListening ...");
	RPC_STATUS status;
	status = RpcMgmtStopServerListening(NULL); // NULL indicates that we want to stop our own server, instead of a remote server

	if (status)
		exit(status);

	Output(hBinding, "Calling: RpcServerUnregisterIf...");
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
	handle_t hBinding = NULL;
	BOOL bAuthBinding = FALSE;
	wprintf(L"[*] RPC Server started.\n");

	RPC_STATUS rpcStatus;
	RPC_WSTR pszProtSeq = (RPC_WSTR)L"ncacn_ip_tcp"; //reinterpret_cast<RPC_WSTR>(L"ncacn_ip_tcp");
	RPC_WSTR pszTCPPort = (RPC_WSTR)L"8989"; //reinterpret_cast<RPC_WSTR>(L"8989");
	//RPC_WSTR pszProtSeq = (RPC_WSTR)L"ncacn_np"; //reinterpret_cast<RPC_WSTR>(L"ncacn_ip_tcp");
	//RPC_WSTR pszTCPPort = (RPC_WSTR)L"\\pipe\\RPCServer1ESCA"; //reinterpret_cast<RPC_WSTR>(L"8989");

	// Create Binding information
	wprintf(L"[*] Create Binding Information using protocol '%s' at '%s'...", pszTCPPort, pszProtSeq, pszTCPPort);
	rpcStatus = RpcServerUseProtseqEp(
		pszProtSeq,						// Use TCP/IP protocol.
		RPC_C_PROTSEQ_MAX_REQS_DEFAULT, // Backlog queue length for TCP/IP.
		pszTCPPort,						// TCP/IP port to use.
		NULL							// No Secuirty Descriptor
	);

	if (rpcStatus) {
		wprintf(L"Failed. Error: %d\n", rpcStatus);
		exit(rpcStatus);
	} else wprintf(L"Success.\n");

	// Registers the Example1 interface.
	wprintf(L"[*] Registering Server interface without registration Flags or SecurityCallback...");
	rpcStatus = RpcServerRegisterIf2(
		Example1_v1_0_s_ifspec,              // Interface to register.
		NULL,                                // Use the MIDL generated entry-point vector.
		NULL,                                // Use the MIDL generated entry-point vector.
		0,									 // No flags.
		RPC_C_LISTEN_MAX_CALLS_DEFAULT,      // Use default number of concurrent calls.
		(unsigned)-1,                        // Infinite max size of incoming data blocks.
		NULL);								 // No security callback.

	if (rpcStatus) {
		wprintf(L"Failed. Error: %s\n", rpcStatus);
		exit(rpcStatus);
	} else wprintf(L"Success.\n");

	if (bAuthBinding) {
		// Obtain SPN for host
		DWORD cSPN;
		TCHAR** ppFoundSPNs;
		DWORD spnStatus;
		RPC_WSTR pszSpn;
		wprintf(L"[*] Getting host SPN...");
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
		if (spnStatus != ERROR_SUCCESS) {
			wprintf(L"Failed. Error: %s\n", rpcStatus);
			exit(spnStatus);
		}
		else wprintf(L"Success.\n");
		pszSpn = (RPC_WSTR)ppFoundSPNs;
		// Register authentication info
		wprintf(L"[*] Registering Auth info with SPN '%s'...", *ppFoundSPNs);
		rpcStatus = RpcServerRegisterAuthInfo(
			pszSpn,					// Server principal name
			RPC_C_AUTHN_WINNT,		// using NTLM as authentication service provider as defined at https://docs.microsoft.com/en-us/windows/win32/rpc/authentication-service-constants
			NULL,					// Use default key function, is ignored for NTLM SSP as per https://docs.microsoft.com/en-us/windows/win32/api/rpcdce/nf-rpcdce-rpcserverregisterauthinfo
			NULL);					// No arg for key function

		if (rpcStatus) {
			wprintf(L"Failed. Error: %s\n", rpcStatus);
			exit(rpcStatus);
		}
		else wprintf(L"Success.\n");
	}
	
	// Start to listen for remote procedure
	// calls for all registered interfaces.
	// This call will not return until
	// RpcMgmtStopServerListening is called.
	wprintf(L"[*] Listening for client connection....\n");
	rpcStatus = RpcServerListen(
		1,                                   // Recommended minimum number of threads.
		RPC_C_LISTEN_MAX_CALLS_DEFAULT,      // Recommended maximum number of threads.
		FALSE);                              // Start listening now.

	if (rpcStatus) {
		wprintf(L"[-] Listening for Client connectrion failed. Error: %s\n", rpcStatus);
		exit(rpcStatus);
	}

	wprintf(L"RPC Server stopped, executing now...\n");
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