// File Example1Client.cpp
#include <iostream>
#include "..\RPC-Interface1\Interface1-Explicit_h.h"

typedef enum _csENDPOINTTYPE {
	TCP_IP,
	NamedPipe,
	LRPC
} CSENDPOINTTYPE;

int main()
{
	wprintf(L"[*] Starting RPC Client!\n");
	RPC_STATUS rpcStatus;
	BOOL bSetBindingAuth = TRUE;
	RPC_WSTR szStringBinding = NULL;
	RPC_WSTR pszProtSeq;
	RPC_WSTR pszTCPPort;
	RPC_WSTR pszTCPHost = (RPC_WSTR)L"GSrv1.SafeAlliance.local";//(RPC_WSTR)L"Spaceland-SRV-1"; //L"WIN10-Client-2"; //reinterpret_cast<RPC_WSTR>(L"localhost");
	RPC_WSTR pszHostSPN = (RPC_WSTR)L"Host/GSRV1";//(RPC_WSTR)L"Host/SPACELAND-SRV-1"; //L"Host/WIN10-CLIENT-2";//
	CSENDPOINTTYPE pwsEndpointType = NamedPipe;

	switch (pwsEndpointType)
	{
	case TCP_IP:
		pszProtSeq = (RPC_WSTR)L"ncacn_ip_tcp";
		pszTCPPort = (RPC_WSTR)L"8989";
		break;
	case NamedPipe:
		pszProtSeq = (RPC_WSTR)L"ncacn_np"; //reinterpret_cast<RPC_WSTR>(L"ncacn_ip_tcp");
		pszTCPPort = (RPC_WSTR)L"\\pipe\\FRPC-NP"; //reinterpret_cast<RPC_WSTR>(L"8989");
		break;
	case LRPC:
		pszProtSeq = (RPC_WSTR)L"ncalrpc";
		pszTCPPort = (RPC_WSTR)L"\\RPC Control\\FRPC-LRPC";
		pszTCPHost = (RPC_WSTR)L"";	// for LRPC we're always connect to a local RPC endpoint. If you add something here your might get error '1707'
		pszHostSPN = (RPC_WSTR)L"LocalAdm"; // NEEDS ADJUSTMENT -- For LRPC set the user that runs the RPC endpoint here, this is a dummy value
		break;
	default:
		pszProtSeq = (RPC_WSTR)L"";
		pszTCPPort = (RPC_WSTR)L"";
		break;
	}
	
	/* Set the quality of service structure */
	RPC_SECURITY_QOS secQos;
	secQos.Version = RPC_C_SECURITY_QOS_VERSION_1;				// use version 1 as documented at https://docs.microsoft.com/en-us/windows/win32/api/rpcdce/ns-rpcdce-rpc_security_qos
	secQos.Capabilities = RPC_C_QOS_CAPABILITIES_MUTUAL_AUTH;	// set mutual authentication
	secQos.IdentityTracking = RPC_C_QOS_IDENTITY_DYNAMIC;		// Dynamic identitiy tracking
	//secQos.ImpersonationType = RPC_C_IMP_LEVEL_ANONYMOUS;		// ANONYMOUS Impersonation type, client does not provide authentication information, server can't obtain client's identity
	//secQos.ImpersonationType = RPC_C_IMP_LEVEL_IDENTIFY;		// IDENTIY Impersonation type, allowing the server to obtain the clien'ts identity and use it for example for ACL checks, but does not allow the server to impersonate the client
	secQos.ImpersonationType = RPC_C_IMP_LEVEL_IMPERSONATE;		// IMPERSONATE Impersonation type, allowing the server to fully impersonate the client on the server's local system but not on remote systems
	//secQos.ImpersonationType = RPC_C_IMP_LEVEL_DELEGATE;		// DELEGATE Impersonation type, allowing the server to fully impersonate the client on the server's local system and on remote systems

	// Creates a string binding handle.
	// Connection is not done here.
	wprintf(L"[*] Create string binding to '%s:%s' using protocol '%s'.\n", pszTCPHost, pszTCPPort, pszProtSeq);
	rpcStatus = RpcStringBindingCompose(
		NULL,           // UUID to bind to.
		pszProtSeq,		// Use TCP/IP protocol.
		pszTCPHost,		// TCP/IPww network address to use.
		pszTCPPort,     // TCP/IP port to use.
		NULL,           // Protocol dependent network options to use.
		&szStringBinding);	// String binding output.

	if (rpcStatus != RPC_S_OK) {
		wprintf(L"[-] RpcStringBindingCompose() failed with status: %d.\n", rpcStatus);
		exit(rpcStatus);
	}

	handle_t hExplicitBinding = NULL;
	// Create a valid binding handle from String
	rpcStatus = RpcBindingFromStringBinding(
		szStringBinding,	// The string binding to validate.
		&hExplicitBinding	// Put the result in the implicit binding
	);						// handle defined in the IDL file.
	if (rpcStatus != RPC_S_OK) {
		wprintf(L"[-] RpcBindingFromStringBinding() failed with status: %d.\n", rpcStatus);
		exit(rpcStatus);
	}

	if (bSetBindingAuth) {
		wprintf(L"[*] Set Binding authentication info to SPN '%s' with an ImpersonationType of '%d'.\n", pszHostSPN, secQos.ImpersonationType);
		rpcStatus = RpcBindingSetAuthInfoEx(
			hExplicitBinding,		// the client's binding handle
			pszHostSPN,				// the server's service principale name (SPN)
			RPC_C_AUTHN_LEVEL_PKT,	// authentication level as defined at https://docs.microsoft.com/en-us/windows/win32/rpc/authentication-level-constants
			RPC_C_AUTHN_WINNT,		// using NTLM as authentication service provider as defined at https://docs.microsoft.com/en-us/windows/win32/rpc/authentication-service-constants
			NULL,					// use current thread credentials
			RPC_C_AUTHZ_NAME,		// authorization based on the provided SPN as defined at https://docs.microsoft.com/en-us/windows/win32/com/com-authorization-constants
			&secQos					// Quality of Service structure
		);

		if (rpcStatus != RPC_S_OK) {
			wprintf(L"[-] RpcBindingSetAuthInfoEx() failed with status: %d.\n", rpcStatus);
			exit(rpcStatus);
		}
	}

	printf("[*] Trying to connect...\n");
	RpcTryExcept
	{
		// Calls the RPC function.
		int retValue = Output(hExplicitBinding, "Hello From Client!");
		//ret = Output(hExplicitBinding, "Triggering Remote Shutdown now...");
		wprintf(L"[+] Value returned from Server is: %d\n", retValue);
		//wprintf(L"[*] Trigger remote Shutdown now...\n");
		//Shutdown(hExplicitBinding);
	}
	RpcExcept(1)
	{
		wprintf(L"Runtime reported exception: %d.\n", RpcExceptionCode());
	}
	RpcEndExcept

	wprintf(L"[*] Shuting down client...\n");
	// Free the memory allocated by a string.
	rpcStatus = RpcStringFree(&szStringBinding);
	if (rpcStatus != RPC_S_OK) {
		wprintf(L"[-] RpcStringFree() failed with status: %d.\n", rpcStatus);
		exit(rpcStatus);
	}

	// Releases binding handle resources and disconnects from the server.
	rpcStatus = RpcBindingFree(&hExplicitBinding);
	if (rpcStatus != RPC_S_OK) {
		wprintf(L"[-] RpcBindingFree() failed with status: %d.\n", rpcStatus);
		exit(rpcStatus);
	}

	// exit successfully
	return 0;
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