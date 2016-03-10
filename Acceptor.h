#pragma once

#include "EventObject.h"

class IOMultiplexer;
class SessionManager;
class Dispatcher;

class Acceptor : public EventObject
{
private:
	IOMultiplexer* _ioMux;
	SessionManager* _ssMgr;
	Dispatcher* _disp;
	
public:
	Acceptor(IOMultiplexer* ioMux, SessionManager* ssMgr, Dispatcher* disp);
	
	bool open(unsigned short port);
	bool open(const char* ip, unsigned short port);

#ifdef WIN32
	bool waitForAccept();
#endif

	void onAccept();
	void onRecv();
	void onSend();
	void onError(int err);
};

