#pragma once
#include <windows.h>

typedef struct _csTOKEN {
	LPWSTR pwsUSERSID;
	TOKEN_TYPE TokenType;
	LPWSTR pwsTokenType;
	LPWSTR pwsTokenImpersonationLevel;
} CSTOKEN, * PCSTOKEN;

typedef enum _csENDPOINTTYPE {
	TCP_IP,
	NamedPipe,
	LRPC
} CSENDPOINTTYPE;