#include "RPC-Util.h"
#include <iostream>
#include <AclAPI.h>

/*
	TOKEN API

*/

BOOL printTokenType(HANDLE hToken, PCSTOKEN pCSToken) {
	PTOKEN_TYPE ptt = NULL;
	DWORD dwSize = 0;
	pCSToken->pwsTokenType = (LPWSTR)L"";
	if (!GetTokenInformation(hToken, TokenType, NULL, 0, &dwSize)
		&& ERROR_INSUFFICIENT_BUFFER != GetLastError())
	{
		return FALSE;
	}
	if (NULL != (ptt = (PTOKEN_TYPE)LocalAlloc(LPTR, dwSize)))
	{
		if (!GetTokenInformation(hToken, TokenType, ptt, dwSize, &dwSize))
		{
			LocalFree((HLOCAL)ptt);
			return FALSE;
		}
		pCSToken->TokenType = (TOKEN_TYPE)*ptt;
		switch (pCSToken->TokenType)
		{
		case TokenImpersonation:
			pCSToken->pwsTokenType = (LPWSTR)L"ImpersonationToken";
			return TRUE;
		case TokenPrimary:
			pCSToken->pwsTokenType = (LPWSTR)L"PrimaryToken";
			return TRUE;
		default:
			wprintf(L"[-] Undefined Token Type. \n");
			return FALSE;
		}

		LocalFree((HLOCAL)ptt);
	}
	return FALSE;
}

BOOL printTokenImpersonationLeven(HANDLE hToken, PCSTOKEN pCSToken) {
	PSECURITY_IMPERSONATION_LEVEL psil = NULL;
	DWORD dwSize = 0;
	pCSToken->pwsTokenImpersonationLevel = (LPWSTR)L"";
	if (!GetTokenInformation(hToken, TokenImpersonationLevel, NULL, 0, &dwSize)
		&& ERROR_INSUFFICIENT_BUFFER != GetLastError())
	{
		return FALSE;
	}
	if (NULL != (psil = (PSECURITY_IMPERSONATION_LEVEL)LocalAlloc(LPTR, dwSize)))
	{
		if (!GetTokenInformation(hToken, TokenImpersonationLevel, psil, dwSize, &dwSize))
		{
			LocalFree((HLOCAL)psil);
			return FALSE;
		}
		switch (*psil)
		{
		case SecurityAnonymous:
			pCSToken->pwsTokenImpersonationLevel = (LPWSTR)L"SecurityAnonymous";
			return TRUE;
		case SecurityIdentification:
			pCSToken->pwsTokenImpersonationLevel = (LPWSTR)L"SecurityIdentification";
			return TRUE;
		case SecurityImpersonation:
			pCSToken->pwsTokenImpersonationLevel = (LPWSTR)L"SecurityImpersonation";
			return TRUE;
		case SecurityDelegation:
			pCSToken->pwsTokenImpersonationLevel = (LPWSTR)L"SecurityDelegation";
			return TRUE;
		default:
			wprintf(L"[-] Undefined Impersonation Level. \n");
			return FALSE;
		}
		LocalFree((HLOCAL)psil);
	}
	return FALSE;
}

BOOL printTokenUserSid(HANDLE hToken, PCSTOKEN pCSToken)
{
	PTOKEN_USER ptu = NULL;
	DWORD dwSize = 0;
	pCSToken->pwsUSERSID = (LPWSTR)L"";
	if (!GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize)
		&& ERROR_INSUFFICIENT_BUFFER != GetLastError())
	{
		return FALSE;
	}
	if (NULL != (ptu = (PTOKEN_USER)LocalAlloc(LPTR, dwSize)))
	{
		LPTSTR StringSid = NULL;
		if (!GetTokenInformation(hToken, TokenUser, ptu, dwSize, &dwSize))
		{
			LocalFree((HLOCAL)ptu);
			return FALSE;
		}
		if (ConvertSidToStringSid(ptu->User.Sid, &StringSid))
		{
			pCSToken->pwsUSERSID = StringSid;
			LocalFree((HLOCAL)StringSid);
			LocalFree((HLOCAL)ptu);
			return TRUE;
		}
		else {
			wprintf(L"[-] Failed to resolve SID to string.\n");
			return FALSE;
		}
		LocalFree((HLOCAL)ptu);
	}
	return FALSE;
}

void ExtractTokenInformation(HANDLE hToken) {
	BOOL result;
	PCSTOKEN pCSToken = new CSTOKEN;
	// Get UserSID
	result = printTokenUserSid(hToken, pCSToken);
	if (!result) wprintf(L" [-]... failed to get Token SID\n");
	else wprintf(L" [+] UserSID: %s\n", pCSToken->pwsUSERSID);
	// GET TokenType
	result = printTokenType(hToken, pCSToken);
	if (!result) wprintf(L" [-]... failed to get Token Type\n");
	else wprintf(L" [+] TokenType: %s\n", pCSToken->pwsTokenType);
	if (pCSToken->TokenType == TokenImpersonation) {
		// GET TokenImpersonationLevel
		result = printTokenImpersonationLeven(hToken, pCSToken);
		if (!result) wprintf(L" [-]... failed to get Token Type\n");
		else wprintf(L" [+] ImpersonationLevel: %s\n", pCSToken->pwsTokenImpersonationLevel);
	}

}

BOOL EnablePriv(HANDLE hToken, LPCTSTR priv)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(NULL, priv, &luid))
	{
		printf("Priv Lookup FALSE\n");
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("Priv Adjust FALSE\n");
		return FALSE;
	}

	return TRUE;
}

// TOKEN API -- END
//

/*
	ACCESS API

*/

void setWindowAccess() {
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
}

void setDesktopAccess() {
	DWORD error;
	wprintf(L"Getting current Desktop..");
	HDESK hDesk = GetThreadDesktop(GetCurrentThreadId());
	if (hDesk) {
		wprintf(L"Success\n");
		wprintf(L"Setting Security of current Desktop...");
		error = SetSecurityInfo(
			hDesk,
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
}

// ACCESS API -- END
//

/*
	COMMAND API

*/

BOOL execCommand(HANDLE hDuppedToken, LPCWSTR command) {
	// -- Execute Process
	BOOL bSuccess;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	//SECURITY_ATTRIBUTES sa;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	//memset(&pi, 0x00, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	wprintf(L"Trying to launch '%s' with CreateProcessWithTokenW... ", command);
	bSuccess = CreateProcessWithTokenW(
		hDuppedToken,
		0,
		command,
		NULL,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi
	);
	if (!bSuccess) {
		wprintf(L"failed (Error: %d).\n", GetLastError());
	}
	else wprintf(L"Success.\n");
	return bSuccess;
}

BOOL execImpersonatedWindowCommand(HANDLE hBinding, LPCWSTR command) {
	BOOL bSuccess;
	HANDLE hClientToken, hDuppedToken;
	wprintf(L"### Exec Command as Impersonated User ###\n\n");
	// -- Set Window & Desktop access rights
	//		This is a dummy implementation and should not be used in production
	setWindowAccess();
	setDesktopAccess();

	// -- Get thread token
	wprintf(L"Trying to get current thread token...");
	bSuccess = OpenThreadToken(GetCurrentThread(), TOKEN_ALL_ACCESS, FALSE, &hClientToken);
	if (!bSuccess) {
		wprintf(L"Failed (Error: %d).\n", GetLastError());
	}
	else wprintf(L"Succeded.\n");
	ExtractTokenInformation(hClientToken);

	// -- Duplicate Token
	wprintf(L"Trying to duplicate impersonated token...");
	bSuccess = DuplicateTokenEx(hClientToken,
		TOKEN_ALL_ACCESS,
		NULL,
		SecurityImpersonation,
		TokenPrimary,
		&hDuppedToken);
	if (!bSuccess) {
		wprintf(L"Failed (Error: %d).\n", GetLastError());
	}
	else wprintf(L"Success!\n");

	// -- Revert back to original thread identity 
	wprintf(L"Reverting back to self...");
	RPC_STATUS rpcStatus = RpcRevertToSelfEx(hBinding);
	if (rpcStatus != RPC_S_OK)
	{
		wprintf(L"Failed to revert back from RPC client impersonation to server identity. Error 0x%x.\n", rpcStatus);
		//exit(rpcStatus);
	}
	else printf("Success.\n");

	// -- Exec command
	bSuccess = execCommand(hDuppedToken, command);
	
	// -- Close Handles
	CloseHandle(hDuppedToken);
	CloseHandle(hClientToken);
	wprintf(L"\n\n### DONE ###\n");
	return bSuccess;
}

// COMMAND API -- END
//