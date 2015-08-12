#pragma once

#define SAFE_DELETE(p) 	if (p) { delete   p; p = nullptr; }
#define SAFE_DELETES(p) 	if (p) { delete[] p; p = nullptr; }

#define MAX_SESSIONS 20480
#define MAX_SEND_BUFFER 1024
#define MAX_RECV_BUFFER 1024

enum EventErrorType
{
	EVENT_ERROR_IO = -1,
	EVENT_ERROR_PARSING = -2,
};

