#include "RPC-Util.h"

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