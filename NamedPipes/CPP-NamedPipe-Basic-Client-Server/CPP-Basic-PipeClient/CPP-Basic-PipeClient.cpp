
#include <windows.h>
#include <iostream>

typedef enum _csIL {
	ANONYMOUS,
	IDENTIFICATION,
	IMPERSONATION,
	DELEGATION
} CSIL;

const int MESSAGE_SIZE = 512;

int main()
{
	HANDLE hFile = NULL;
	BOOL bPipeRead = TRUE;
	LPWSTR pReadBuf[MESSAGE_SIZE] = { 0 };
	LPDWORD pdwBytesRead = { 0 };
	DWORD dwFlags = 0;
	BOOL bEnforceSQOS = TRUE;
	CSIL csImpersonationLevel = IMPERSONATION;
	
	// explicit set SQOS if enabled
	if(bEnforceSQOS) dwFlags = SECURITY_SQOS_PRESENT;
	// set Impersonation Level
	switch (csImpersonationLevel)
	{
	case ANONYMOUS:
		dwFlags |= SECURITY_ANONYMOUS;
		break;
	case IDENTIFICATION:
		dwFlags |= SECURITY_IDENTIFICATION;
		break;
	case IMPERSONATION:
		dwFlags |= SECURITY_IMPERSONATION;
		break;
	case DELEGATION:
		dwFlags |= SECURITY_DELEGATION;
		break;
	default:
		dwFlags |= SECURITY_IDENTIFICATION;
		break;
	}
	// Read name pipe to from STDIN
	wchar_t wcInputPipeName[200];
	wprintf(L"[*] Pipe name to connect to (max 200 chars): ");
	std::wcin.getline(wcInputPipeName, 200);
	int iInputLen = wcslen(wcInputPipeName);
	if ( iInputLen > 200) {
		wprintf(L"[-] More than 200 chars (%d).. exiting.\n", iInputLen);
		return 1;
	}

	// Connect to pipe server
	wprintf(L"[*] Connecting to %s ...", wcInputPipeName);
	//hFile = CreateFile(pipeName, GENERIC_READ , 0, NULL, OPEN_EXISTING, 0, NULL);
	hFile = CreateFile(wcInputPipeName, GENERIC_READ, 0, NULL, OPEN_EXISTING, dwFlags, NULL);
	if (hFile != INVALID_HANDLE_VALUE) wprintf(L"Success.\n");
	else wprintf(L"Error: %d.", GetLastError());

	// Read from pipe
	while (bPipeRead) {
		wprintf(L"[*] Reading from pipe...");
		bPipeRead = ReadFile(hFile, pReadBuf, MESSAGE_SIZE, pdwBytesRead, NULL);
		if (!bPipeRead) wprintf(L"Error: %d.\n", GetLastError());
		else {
			wprintf(L"Success. Read %d bytes, Received: %s\n", pdwBytesRead, pReadBuf);
		}
		// Sleep for some time just to add some latency
		Sleep(2000);
	}
	// Close handle
	CloseHandle(hFile);
	return 0;
}
