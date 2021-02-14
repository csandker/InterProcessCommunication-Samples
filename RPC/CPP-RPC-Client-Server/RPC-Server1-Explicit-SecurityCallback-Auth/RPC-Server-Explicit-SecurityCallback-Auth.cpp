
#include <iostream>
#include <windows.h>
#include <sddl.h>
#include <ntdsapi.h>
#include <aclapi.h> // For Window Security
#include "..\RPC-Interface1\Interface1-Explicit_h.h"

#include "..\RPC-Util\Access.cpp"
#include "..\RPC-Util\Token.cpp"

//typedef struct _csTOKEN {
//	LPWSTR pwsUSERSID;
//	TOKEN_TYPE TokenType;
//	LPWSTR pwsTokenType;
//	LPWSTR pwsTokenImpersonationLevel;
//} CSTOKEN, *PCSTOKEN;
//
//typedef enum _csENDPOINTTYPE {
//	TCP_IP,
//	NamedPipe,
//	LRPC
//} CSENDPOINTTYPE;
//
//BOOL EnablePriv(HANDLE hToken, LPCTSTR priv)
//{
//	TOKEN_PRIVILEGES tp;
//	LUID luid;
//
//	if (!LookupPrivilegeValue(NULL, priv, &luid))
//	{
//		printf("Priv Lookup FALSE\n");
//		return FALSE;
//	}
//
//	tp.PrivilegeCount = 1;
//	tp.Privileges[0].Luid = luid;
//	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
//	if (!AdjustTokenPrivileges(
//		hToken,
//		FALSE,
//		&tp,
//		sizeof(TOKEN_PRIVILEGES),
//		(PTOKEN_PRIVILEGES)NULL,
//		(PDWORD)NULL))
//	{
//		printf("Priv Adjust FALSE\n");
//		return FALSE;
//	}
//
//	return TRUE;
//}

//BOOL printTokenType(HANDLE hToken, PCSTOKEN pCSToken) {
//	PTOKEN_TYPE ptt = NULL;
//	DWORD dwSize = 0;
//	pCSToken->pwsTokenType = (LPWSTR)L"";
//	if (!GetTokenInformation(hToken, TokenType, NULL, 0, &dwSize)
//		&& ERROR_INSUFFICIENT_BUFFER != GetLastError())
//	{
//		return FALSE;
//	}
//	if (NULL != (ptt = (PTOKEN_TYPE)LocalAlloc(LPTR, dwSize)))
//	{
//		if (!GetTokenInformation(hToken, TokenType, ptt, dwSize, &dwSize))
//		{
//			LocalFree((HLOCAL)ptt);
//			return FALSE;
//		}
//		pCSToken->TokenType = (TOKEN_TYPE)*ptt;
//		switch (pCSToken->TokenType)
//		{
//		case TokenImpersonation:
//			pCSToken->pwsTokenType = (LPWSTR)L"ImpersonationToken";
//			return TRUE;
//		case TokenPrimary:
//			pCSToken->pwsTokenType = (LPWSTR)L"PrimaryToken";
//			return TRUE;
//		default:
//			wprintf(L"[-] Undefined Token Type. \n");
//			return FALSE;
//		}
//
//		LocalFree((HLOCAL)ptt);
//	}
//	return FALSE;
//}
//
//BOOL printTokenImpersonationLeven(HANDLE hToken, PCSTOKEN pCSToken) {
//	PSECURITY_IMPERSONATION_LEVEL psil = NULL;
//	DWORD dwSize = 0;
//	pCSToken->pwsTokenImpersonationLevel = (LPWSTR)L"";
//	if (!GetTokenInformation(hToken, TokenImpersonationLevel, NULL, 0, &dwSize)
//		&& ERROR_INSUFFICIENT_BUFFER != GetLastError())
//	{
//		return FALSE;
//	}
//	if (NULL != (psil = (PSECURITY_IMPERSONATION_LEVEL)LocalAlloc(LPTR, dwSize)))
//	{
//		if (!GetTokenInformation(hToken, TokenImpersonationLevel, psil, dwSize, &dwSize))
//		{
//			LocalFree((HLOCAL)psil);
//			return FALSE;
//		}
//		switch (*psil)
//		{
//		case SecurityAnonymous:
//			pCSToken->pwsTokenImpersonationLevel = (LPWSTR)L"SecurityAnonymous";
//			return TRUE;
//		case SecurityIdentification:
//			pCSToken->pwsTokenImpersonationLevel = (LPWSTR)L"SecurityIdentification";
//			return TRUE;
//		case SecurityImpersonation:
//			pCSToken->pwsTokenImpersonationLevel = (LPWSTR)L"SecurityImpersonation";
//			return TRUE;
//		case SecurityDelegation:
//			pCSToken->pwsTokenImpersonationLevel = (LPWSTR)L"SecurityDelegation";
//			return TRUE;
//		default:
//			wprintf(L"[-] Undefined Impersonation Level. \n");
//			return FALSE;
//		}
//		LocalFree((HLOCAL)psil);
//	}
//	return FALSE;
//}
//
//BOOL printTokenUserSid(HANDLE hToken, PCSTOKEN pCSToken)
//{
//	PTOKEN_USER ptu = NULL;
//	DWORD dwSize = 0;
//	pCSToken->pwsUSERSID = (LPWSTR)L"";
//	if (!GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize)
//		&& ERROR_INSUFFICIENT_BUFFER != GetLastError())
//	{
//		return FALSE;
//	}
//	if (NULL != (ptu = (PTOKEN_USER)LocalAlloc(LPTR, dwSize)))
//	{
//		LPTSTR StringSid = NULL;
//		if (!GetTokenInformation(hToken, TokenUser, ptu, dwSize, &dwSize))
//		{
//			LocalFree((HLOCAL)ptu);
//			return FALSE;
//		}
//		if (ConvertSidToStringSid(ptu->User.Sid, &StringSid))
//		{
//			pCSToken->pwsUSERSID = StringSid;
//			LocalFree((HLOCAL)StringSid);
//			LocalFree((HLOCAL)ptu);
//			return TRUE;
//		}
//		else {
//			wprintf(L"[-] Failed to resolve SID to string.\n");
//			return FALSE;
//		}
//		LocalFree((HLOCAL)ptu);
//	}
//	return FALSE;
//}
//
//
//void ExtractTokenInformation(HANDLE hToken) {
//	BOOL result;
//	PCSTOKEN pCSToken = new CSTOKEN;
//	// Get UserSID
//	result = printTokenUserSid(hToken, pCSToken);
//	if (!result) wprintf(L" [-]... failed to get Token SID\n");
//	else wprintf(L" [+] UserSID: %s\n", pCSToken->pwsUSERSID);
//	// GET TokenType
//	result = printTokenType(hToken, pCSToken);
//	if (!result) wprintf(L" [-]... failed to get Token Type\n");
//	else wprintf(L" [+] TokenType: %s\n", pCSToken->pwsTokenType);
//	if (pCSToken->TokenType == TokenImpersonation) {
//		// GET TokenImpersonationLevel
//		result = printTokenImpersonationLeven(hToken, pCSToken);
//		if (!result) wprintf(L" [-]... failed to get Token Type\n");
//		else wprintf(L" [+] ImpersonationLevel: %s\n", pCSToken->pwsTokenImpersonationLevel);
//	}
//
//}

void LaunchPrg(HANDLE hClientToken) {
	// open cmd.exe as client
	STARTUPINFO	si = {};
	PROCESS_INFORMATION pi = {};
	DWORD gle = 0;
	HANDLE duped_token;
	wchar_t command[] = L"C:\\Windows\\system32\\calc.exe";
	wprintf(L"Launching cmd.exe as impersonated client...");

	try {
		// create impersonation token
		BOOL result = DuplicateTokenEx(hClientToken,
			TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_DEFAULT | TOKEN_ADJUST_SESSIONID | TOKEN_IMPERSONATE,
			NULL,
			SecurityImpersonation,
			TokenImpersonation,
			&duped_token);

		if (!result) {
			printf("[-] DuplicateTokenEx failed: %d\n", GetLastError());
		}

		if (CreateProcessWithTokenW(duped_token, LOGON_WITH_PROFILE, NULL,
			command, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
			wprintf(L"Success?...Waiting 10 secs\n");

			Sleep(10);
		}
		else {
			wprintf(L"Failed\n");
			gle = GetLastError();
			wprintf(L"Error code: %s\n", gle);
		}
	}
	catch (...) {
		gle = GetLastError();
		wprintf(L"Error code: %s\n", gle);
	}
	CloseHandle(hClientToken);
}

// Naive security callback.
RPC_STATUS CALLBACK SecurityCallback(RPC_IF_HANDLE hInterface, void* pBindingHandle)
{
	wprintf(L"Inside Security Callback");
	return RPC_S_OK; // Always allow anyone.
}

// Future server function.
int Output(
	/* [in] */ handle_t hBinding,
	/* [string][in] */ const char* pszOutput)
{
	HANDLE hGClientToken;
	HANDLE hGDuppedToken;
	
	printf("\n[~] Client Message: %s\n", pszOutput);
	if (true) {
		setWindowAccess();
		setDesktopAccess();
		// Try to impersonate client
		wprintf(L"Trying to impersonate client...");
		RPC_STATUS status = RpcImpersonateClient(hBinding);
		if (status != RPC_S_OK) {
			printf("failed.Status: %d\n", status);
			//Shutdown(hBinding);
			return 10;
		}
		else printf("Success!\n");
		// Get thread token
		printf("Trying to get current thread token...");
		BOOL bThrTokenOpened = OpenThreadToken(GetCurrentThread(), TOKEN_ALL_ACCESS, FALSE, &hGClientToken);
		if (!bThrTokenOpened) {
			printf("Failed.\n");
		}
		else printf("Succeded.\n");
		ExtractTokenInformation(hGClientToken);
		// Duplicate Token
		wprintf(L"Trying to duplicate impersonated token...");
		BOOL res5 = DuplicateTokenEx(hGClientToken,
			TOKEN_ALL_ACCESS,
			NULL,
			SecurityImpersonation,
			TokenPrimary,
			&hGDuppedToken);
		if (res5) wprintf(L"Success!\n");
		else wprintf(L"Failure\n");
		// revert back to server thread identity 
		printf("Reverting back to self...");
		RPC_STATUS rpcStatus = RpcRevertToSelfEx(hBinding);
		if (rpcStatus)
		{
			wprintf(L"Failed to revert back from RPC client impersonation to server identity. Error 0x%x.\n", rpcStatus);
			//exit(rpcStatus);
		}
		else printf("Success.\n");
		// -- Execute Process
		printf("Trying to launch cmd.exe with CreateProcessWithTokenW... ");
		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		wchar_t command[] = L"C:\\Windows\\System32\\cmd.exe";
		//SECURITY_ATTRIBUTES sa;
		ZeroMemory(&si, sizeof(STARTUPINFO));
		ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
		//memset(&pi, 0x00, sizeof(PROCESS_INFORMATION));
		si.cb = sizeof(STARTUPINFO);
		BOOL bSucc = CreateProcessWithTokenW(
			hGDuppedToken,
			0,
			command,
			NULL,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&si,
			&pi
		);
		if (!bSucc) printf("failed. Error: %d\n", GetLastError());
		else wprintf(L"Success.\n");
		// -- Close Handles
		CloseHandle(hGDuppedToken);
		CloseHandle(hGClientToken);
	}
	// -- Return value
	return 5;

	/*
	wprintf(L"\nGet Current Session ID...");
	DWORD sessionID = 12;
	BOOL bRes = ProcessIdToSessionId(GetCurrentProcessId(), &sessionID);
	if (bRes) wprintf(L"SessionID: %d\n", sessionID);
	else wprintf(L"Failed. Error: %d\n", GetLastError());
	*/

	DWORD error;
	wprintf(L"Gettinng current process Window...");
	HWINSTA hWinSta = GetProcessWindowStation();
	if (hWinSta) {
		wprintf(L"Success.\n");
		wprintf(L"Setting Security of current Proccess Window...");
		error = SetSecurityInfo(hWinSta,
			SE_WINDOW_OBJECT,
			DACL_SECURITY_INFORMATION,
			NULL,	// don't set the ownerSID
			NULL,	// don't set the primary GROUP
			NULL,	// DACL pointer, NULL=> full access to everyone
			NULL	// no setting SACL
		);
		if (error == ERROR_SUCCESS) wprintf(L"Success.\n");
		else wprintf(L"Error: %d.\n", error);
	}
	else wprintf(L"Error: %d.\n", GetLastError());
	
	wprintf(L"Getting current Desktop..");
	HDESK hDesk = GetThreadDesktop(GetCurrentThreadId());
	if (hDesk) { 
		wprintf(L"Success\n");
		wprintf(L"Setting Security of current Desktop...");
		error = SetSecurityInfo(hDesk,
			SE_WINDOW_OBJECT,
			DACL_SECURITY_INFORMATION,
			NULL,	// don't set the ownerSID
			NULL,	// don't set the primary GROUP
			NULL,	// DACL pointer, NULL=> full access to everyone
			NULL	// no setting SACL
		);
		if (error == ERROR_SUCCESS) wprintf(L"Success\n");
		else wprintf(L"Error: %d\n", error);
	}
	else wprintf(L"Error: %d\n", GetLastError());

	/*
	HANDLE hGServerToken;
	wprintf(L"\nGet Server's thread access token...");
	BOOL hGSrvAccess = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hGServerToken);
	if (!hGSrvAccess) {
		printf("Error: %s\n", GetLastError());
	}
	else printf("Succeded.\n");
	*/
	
	//szReturn = "Your Output was: ";

	/*
	wprintf(L"Trying calling with own token...");
	HANDLE hServerToken2;
	BOOL bThrTokenOpened2 = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hServerToken2);
	if (!bThrTokenOpened2) {
		DWORD eii = GetLastError();
		wprintf(L"Error OpenThreadToken: %d\n", eii);
	}
	ExtractTokenInformation(hServerToken2);

	PROCESS_INFORMATION pi2;
	STARTUPINFO si2;
	//SECURITY_ATTRIBUTES sa;
	ZeroMemory(&si2, sizeof(STARTUPINFO));
	ZeroMemory(&pi2, sizeof(PROCESS_INFORMATION));
	memset(&pi2, 0x00, sizeof(PROCESS_INFORMATION));
	si2.cb = sizeof(STARTUPINFO);
	BOOL ret8 = TRUE;//CreateProcessWithTokenW(hServerToken2, LOGON_NETCREDENTIALS_ONLY, L"C:\\Windows\\System32\\cmd.exe", NULL, CREATE_NEW_CONSOLE, NULL, NULL, &si2, &pi2);
	if (!ret8) {
		DWORD klqoo = GetLastError();
		wprintf(L"Error %d\n", klqoo);
	}
	else printf("Successy!!\n");
	*/

	// Try to impersonate client
	wprintf(L"Trying to impersonate client...");
	RPC_STATUS status = RpcImpersonateClient(hBinding);
	if (status != RPC_S_OK) {
		printf("failed.Status: %d\n", status);
		//Shutdown(hBinding);
		return 10;
	}
	else printf("Success!\n");


	/*
	// WORKING SPAWNING cmd.exe with server user (not impersonating)

	printf("Trying to launch cmd.exe with CreateProcess...");
	PROCESS_INFORMATION processInformation = { 0 };
	STARTUPINFO startupInfo = { 0 };
	startupInfo.cb = sizeof(startupInfo);

	printf("Creating process cmd.exe ...");
	// Create the process
	wchar_t command[] = L"C:\\Windows\\system32\\cmd.exe";
	BOOL result = CreateProcess(NULL, command,
		NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE,
		NULL, NULL, &startupInfo, &processInformation);
	if (!result) {
		printf("Failed.\n");
		return 1;
	}
	else printf("Success.\n");
	// Successfully created the process.  Wait for it to finish.
	WaitForSingleObject(processInformation.hProcess, INFINITE);
	//CloseHandle(processInformation.hProcess);
	//CloseHandle(processInformation.hThread);

	// WORKING --END

	*/

	// Get token from current thread
	//HANDLE hClientToken;
	printf("Trying to get current thread token...");
	BOOL bThrTokenOpened = OpenThreadToken(GetCurrentThread(), TOKEN_ALL_ACCESS, FALSE, &hGClientToken);
	if (!bThrTokenOpened) {
		printf("Failed.\n");
	}
	else printf("Succeded.\n");
	ExtractTokenInformation(hGClientToken);





	/*
	PSID ppUserSid;
	PSID ppGroupSid;
	wchar_t* foo[500] = {0};
	//HRESULT res = GetTokenSids(hClientToken, &ppUserSid, &ppGroupSid);
	BOOL res3 = GetUserSid(hClientToken, &ppUserSid);
	printf("Result: %d \n", res3);
	BOOL res4 = ConvertSidToStringSid(ppUserSid, foo);
	if (!res4) printf("Failed to resovle SID to String\n");
	else {
		printf("Success to resolve SID...\n");
		printf("SID: %s", foo);
	}
	int a = 4;
	//printf("User SIDs: %s", *ppUserSid->pSid)
	*/

	// Duplicate Token
	//HANDLE duped_token;
	wprintf(L"Trying to duplicate impersonated token...");
	BOOL res5 = DuplicateTokenEx(hGClientToken,
		TOKEN_ALL_ACCESS,
		NULL,
		SecurityImpersonation,
		TokenPrimary,
		&hGDuppedToken);
	if (res5) wprintf(L"Success!\n");
	else wprintf(L"Failure\n");



	//enable privileges
	//BOOL bPrivsEn;
	/*
	HANDLE currenThreadToken;
	printf("Open current Thread token...");
	BOOL bThrTokenOpened2 = OpenThreadToken(GetCurrentThread(), TOKEN_ALL_ACCESS, FALSE, &currenThreadToken);
	if (!bThrTokenOpened2) {
		printf("Error: %d\n", GetLastError());
	}
	else printf("Succeded.\n");
	*/
	/*
	printf("Enabling Privs with the user:\n");
	ExtractTokenInformation(hGDuppedToken);
	printf("\n");
	wprintf(L"Enable Privilege: SE_IMPERSONATE_NAME....");
	bPrivsEn = EnablePriv(hGDuppedToken, SE_IMPERSONATE_NAME);
	if (bPrivsEn) wprintf(L"Success.\n");
	else wprintf(L"failed.\n");
	wprintf(L"Enable Privilege: SE_ASSIGNPRIMARYTOKEN_NAME....");
	bPrivsEn = EnablePriv(hGDuppedToken, SE_ASSIGNPRIMARYTOKEN_NAME);
	if (bPrivsEn) wprintf(L"Success.\n");
	else wprintf(L"failed.\n");
	wprintf(L"Enable Privilege: SE_INCREASE_QUOTA_NAME....");
	bPrivsEn = EnablePriv(hGDuppedToken, SE_INCREASE_QUOTA_NAME);
	if (bPrivsEn) wprintf(L"Success.\n");
	else wprintf(L"failed.\n");
	*/

	printf("Reverting back to self...");
	// revert back to server thread identity 
	RPC_STATUS rpcStatus = RpcRevertToSelfEx(hBinding);
	if (rpcStatus)
	{
		wprintf(L"Failed to revert back from RPC client impersonation to server identity. Error 0x%x.\n", rpcStatus);
		//exit(rpcStatus);
	}
	else printf("Success.\n");

	/*
	printf("Enabling Privs with the user:\n");
	ExtractTokenInformation(hGServerToken);
	LPCWSTR pwsPrivileges[6] = { SE_IMPERSONATE_NAME, SE_ASSIGNPRIMARYTOKEN_NAME,
		SE_INCREASE_QUOTA_NAME, SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME,
		SE_ENABLE_DELEGATION_NAME,
		SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME,
	};
	for (LPCWSTR pwsPrivilege : pwsPrivileges)
	{
		wprintf(L"Enable Privilege: %s ....", pwsPrivilege);
		bPrivsEn = EnablePriv(hGServerToken, pwsPrivilege);
		if (bPrivsEn) wprintf(L"Success.\n");
		else wprintf(L"failed. Error: %d\n", GetLastError());
	}
	*/


	// TEST 
	// TRY number #1

	printf("Trying to launch cmd.exe with CreateProcessWithTokenW... ");
	//PROCESS_INFORMATION processInformation2 = { 0 };
	//STARTUPINFO startupInfo2 = { 0 };
	//BOOL res = CreateProcessWithTokenW(hClientToken, LOGON_WITH_PROFILE, command1, command1,
	//	CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo2, &processInformation2);
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	wchar_t command[] = L"C:\\Windows\\System32\\cmd.exe";
	//SECURITY_ATTRIBUTES sa;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	//memset(&pi, 0x00, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	BOOL bSucc = CreateProcessWithTokenW(
		hGDuppedToken,
		0,
		command,
		NULL,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi
	);
	if (!bSucc) printf("failed. Error: %d\n", GetLastError());
	else wprintf(L"Success.\n");
	//ExtractTokenInformation(hGDuppedToken);
	//WaitForSingleObject(pi.hProcess, INFINITE); // use this if you want to wait for the proccess to complete



	/*
	wprintf(L"Try Number #2...");
	BOOL res = CreateProcessWithTokenW(
		hGDuppedToken,				// Duplicated Client token
		0,							// No logon Flags (we don't want to set the logon registy)
		L"C:\\Windows\\System32\\cmd.exe",					// let's not use the lpApplicationName, but only the lpCommandLine
		NULL,						// lpCommandLine to execute
		CREATE_NEW_CONSOLE,			// get a new fresh console, not inheriting the parent's console
		NULL,						// no environment block needed
		L"C:\\",						// inherit the current directory from this calling proccess
		&si,						// startup information struct
		&pi);						// proccess informaiton struct

	if (!res) {
		DWORD gle = GetLastError();
		printf("Error from next CreateProcessWithTokenW: %d\n", gle);
	}
	else printf("Success. \n");
	*/


	/*
	// Try 3
	wprintf(L"Try number #3 ...");
	PROCESS_INFORMATION pi3;
	STARTUPINFO si3;
	//SECURITY_ATTRIBUTES sa;
	//sa.bInheritHandle = false;
	//sa.nLength = sizeof(sa);
	//sa.lpSecurityDescriptor = nullptr;

	ZeroMemory(&si3, sizeof(STARTUPINFO));
	ZeroMemory(&pi3, sizeof(PROCESS_INFORMATION));
	//memset(&pi3, 0x00, sizeof(PROCESS_INFORMATION));
	si3.cb = sizeof(STARTUPINFO);
	//si3.lpDesktop = (LPWSTR)L"winsta0\\default";
	si3.wShowWindow = SW_SHOWNORMAL;
	si3.dwFlags = STARTF_USESHOWWINDOW;
	wchar_t command3[] = L"C:\\Windows\\System32\\notepad.exe";
	wchar_t command4[] = L"cmd.exe";
	BOOL res6 = CreateProcessAsUser(
		hGDuppedToken,			// duplicated primary token
		NULL,					// appName
		command3,				// cmdLine
		NULL,					// no security descriptor for proccess
		NULL,					// no security descriptor for thread
		FALSE,					// don't inherit handles from the calling process
		CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,		// creation flags
		nullptr,					// inherit environment from the calling process
		L"C:\\",					// spawn in the directory of the calling process
		&si3,
		&pi3
	);
	if (res6) {
		WaitForSingleObject(pi3.hProcess, INFINITE);

		Sleep(5);
		wprintf(L"Success. PID: %d \n", pi3.dwProcessId);
	}
	else {
		DWORD gle2 = GetLastError();
		wprintf(L"nope: %d\n", gle2);
	}
	*/

	// Try number #4 - assign primary token
	//SetThreadToken

	CloseHandle(hGDuppedToken);
	CloseHandle(hGClientToken);

	// TEST --END

	//Shutdown(hBinding);
	/***
	if (bThrTokenOpened) {
		try
		{
			wprintf(L"Trying to extract token information\n");
			//ExtractTokenInformation(hClientToken);
			//std::string foo = sid_from_token_info<TOKEN_USER, TokenUser>(hClientToken);
			//wprintf(L"SID IS: %s", foo);
		}
		catch (...)
		{
			wprintf(L"Error while trying to extract token information\n");
		}
		LaunchPrg(hClientToken);

		/
		// open cmd.exe as client
		STARTUPINFO	si = {};
		PROCESS_INFORMATION pi = {};
		DWORD gle = 0;
		HANDLE duped_token;
		wchar_t command[] = L"C:\\Windows\\system32\\calc.exe";
		wprintf(L"Launching cmd.exe as impersonated client...");

		try {
			// create impersonation token
			BOOL result = DuplicateTokenEx(hClientToken,
				TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_DEFAULT | TOKEN_ADJUST_SESSIONID | TOKEN_IMPERSONATE,
				NULL,
				SecurityImpersonation,
				TokenImpersonation,
				&duped_token);

			if (!result) {
				printf("[-] DuplicateTokenEx failed: %d\n", GetLastError());
			}

			if (CreateProcessWithTokenW(duped_token, LOGON_WITH_PROFILE, L"C:\\Windows\\system32\\calc.exe",
				NULL, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
				wprintf(L"Success?...Waiting 10 secs\n");

				Sleep(10);
			}
			else {
				wprintf(L"Failed\n");
				gle = GetLastError();
				wprintf(L"Error code: %s\n", gle);
			}
		}
		catch (...) {
			gle = GetLastError();
			wprintf(L"Error code: %s\n", gle);
		}
		CloseHandle(hClientToken);
		/
	}
	else {
		DWORD gle = GetLastError();
		wprintf(L"Failed to open thread token for impersonated RPC client. Error 0x%x.\n", gle);
		return HRESULT_FROM_WIN32(gle);
	}

	***/

	return 5;
}


void Shutdown(
	/* [in] */ handle_t hBinding)
{
	wprintf(L"Calling: RpcMgmtStopServerListening ...\n");
	RPC_STATUS status;
	status = RpcMgmtStopServerListening(NULL); // NULL indicates that we want to stop our own server, instead of a remote server

	if (status)
		exit(status);

	wprintf(L"Calling: RpcServerUnregisterIf...\n");
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
	CSENDPOINTTYPE pwsEndpointType = LRPC;
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
	wprintf(L"[*] Registering Server interface.\n");
	rpcStatus = RpcServerRegisterIf2(
		Example1_v1_0_s_ifspec,              // Interface to register.
		NULL,                                // Use the MIDL generated entry-point vector.
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