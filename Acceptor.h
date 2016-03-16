#pragma once

#include "EventObject.h"

#define MAX_ACCEPT_BUFFER 1024

class IOMultiplexer;
class SessionManager;
class Dispatcher;

class Acceptor : public EventObject
{
private:
	IOMultiplexer* _ioMux;
	SessionManager* _ssMgr;
	Dispatcher* _disp;

#ifdef WIN32
	char _acceptBuffer[MAX_ACCEPT_BUFFER];
	Socket _acceptSocket;
#endif
	
public:
	Acceptor(IOMultiplexer* ioMux, SessionManager* ssMgr, Dispatcher* disp);
	
	bool open(unsigned short port);
	bool open(const char* ip, unsigned short port);

#ifdef WIN32
	bool waitForAccept();
#endif

	void onAccept();
	void onError(int err);
};

