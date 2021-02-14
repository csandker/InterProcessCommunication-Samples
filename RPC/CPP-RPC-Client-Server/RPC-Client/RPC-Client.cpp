// File Example1Client.cpp
#include <iostream>
#include "..\RPC-Interface1\Example1_h.h"

int main()
{
	RPC_STATUS status;
	RPC_WSTR szStringBinding = NULL;
	RPC_WSTR pszProtSeq = (RPC_WSTR)L"ncacn_ip_tcp"; //reinterpret_cast<RPC_WSTR>(L"ncacn_ip_tcp");
	RPC_WSTR pszTCPHost = (RPC_WSTR)L"localhost"; //reinterpret_cast<RPC_WSTR>(L"localhost");
	RPC_WSTR pszTCPPort = (RPC_WSTR)L"8989"; //reinterpret_cast<RPC_WSTR>(L"8989");

	// Creates a string binding handle.
	// Connection is not done here.
	status = RpcStringBindingCompose(
		NULL,           // UUID to bind to.
		pszProtSeq,		// Use TCP/IP protocol.
		pszTCPHost,		// TCP/IP network address to use.
		pszTCPPort,     // TCP/IP port to use.
		NULL,           // Protocol dependent network options to use.
		&szStringBinding);	// String binding output.

	if (status)
		exit(status);

	handle_t hBinding = NULL;
	// Create a valid binding handle from String
	status = RpcBindingFromStringBinding(
		szStringBinding,	// The string binding to validate.
		&hBinding			// Put the result in the implicit binding
	);						// handle defined in the IDL file.

	if (status)
		exit(status);

	RpcTryExcept
	{
		// Calls the RPC function. The hExample1Binding binding handle
		// is used implicitly (as defined in the Interface IDL file).
		// Connection is done here.
		int ret = Output(hBinding, "Hello From Client!");
		ret = Output(hBinding, "Triggering Remote Shutdown now...");
		std::cout << "Return value was: " << ret << std::endl;
		Shutdown(hBinding);
	}
		RpcExcept(1)
	{
		std::cerr << "Runtime reported exception " << RpcExceptionCode()
			<< std::endl;
	}
	RpcEndExcept

	// Free the memory allocated by a string.
	status = RpcStringFree(
		&szStringBinding); // String to be freed.

	if (status)
		exit(status);

	// Releases binding handle resources and disconnects from the server.
	status = RpcBindingFree(
		&hBinding); // Frees the implicit binding handle defined in the IDL file.

	if (status)
		exit(status);
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