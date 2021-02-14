
#include <iostream>
#include "..\RPC-Interface1\Interface1-Implicit_h.h"


// Future server function.
int Output(
	/* [string][in] */ const char* pszOutput)
{
	std::cout << pszOutput << std::endl;
	return 5;
}


void Shutdown()
{
	Output("Calling: RpcMgmtStopServerListening ...");
	RPC_STATUS status;
	status = RpcMgmtStopServerListening(NULL); // NULL indicates that we want to stop our own server, instead of a remote server

	if (status)
		exit(status);

	Output("Calling: RpcServerUnregisterIf...");
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
	Output("Hello From RPC Server!");

	RPC_STATUS rpcStatus;
	RPC_WSTR pszProtSeq = (RPC_WSTR)L"ncacn_ip_tcp"; //reinterpret_cast<RPC_WSTR>(L"ncacn_ip_tcp");
	RPC_WSTR pszTCPPort = (RPC_WSTR)L"8989"; //reinterpret_cast<RPC_WSTR>(L"8989");

	// Create Binding information
	Output("Creating Binding Information.");
	rpcStatus = RpcServerUseProtseqEp(
		pszProtSeq,						// Use TCP/IP protocol.
		RPC_C_PROTSEQ_MAX_REQS_DEFAULT, // Backlog queue length for TCP/IP.
		pszTCPPort,						// TCP/IP port to use.
		NULL							// No Secuirty Descriptor
	);

	if (rpcStatus)
		exit(rpcStatus);

	// Registers the Example1 interface.
	Output("Registering Server interface.");
	rpcStatus = RpcServerRegisterIf2(
		Example1_v1_0_s_ifspec,              // Interface to register.
		NULL,                                // Use the MIDL generated entry-point vector.
		NULL,                                // Use the MIDL generated entry-point vector.
		0,									 // No Flags.
		RPC_C_LISTEN_MAX_CALLS_DEFAULT,      // Use default number of concurrent calls.
		(unsigned)-1,                        // Infinite max size of incoming data blocks.
		NULL);								 // No security callback.

	if (rpcStatus)
		exit(rpcStatus);

	// Start to listen for remote procedure
	// calls for all registered interfaces.
	// This call will not return until
	// RpcMgmtStopServerListening is called.
	Output("Listening for client connection.");
	rpcStatus = RpcServerListen(
		1,                                   // Recommended minimum number of threads.
		RPC_C_LISTEN_MAX_CALLS_DEFAULT,      // Recommended maximum number of threads.
		FALSE);                              // Start listening now.

	if (rpcStatus)
		exit(rpcStatus);

	Output("RPC Server stopped, executing now...");
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