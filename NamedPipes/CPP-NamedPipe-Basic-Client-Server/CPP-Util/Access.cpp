#include <Windows.h>
#include <iostream>
#include <AclAPI.h>

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
}