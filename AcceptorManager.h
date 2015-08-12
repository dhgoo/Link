#pragma once

class IOMultiplexer;
class SessionManager;
class Dispatcher;
class Acceptor;

class AcceptorManager
{
	typedef list<Acceptor* > AcceptorList;

private:
	IOMultiplexer* _ioMux;
	SessionManager* _ssMgr;
	AcceptorList _activatedAcceptors;

public:
	AcceptorManager(IOMultiplexer* ioMux, SessionManager* ssMgr);
	~AcceptorManager();

	Acceptor* create(Dispatcher* disp);
};

