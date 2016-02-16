#include "Precompiled.h"
#include "Link.h"
#include "Dispatcher.h"
#include "Socket.h"
#include "IOMultiplexer.h"
#include "Session.h"
#include "SessionManager.h"
#include "NetworkThread.h"
#include "Acceptor.h"
#include "AcceptorManager.h"

Link::Link()
	: _ioMultiplexer(nullptr)
	, _sessionManager(nullptr)
	, _networkThread(nullptr)
	, _acceptorManager(nullptr)
	, _initialized(false) 
	, _running(false)
{
}

Link::~Link()
{
	stop();

	close();

	if (_acceptorManager) 
		delete _acceptorManager;

	if (_networkThread) 
		delete _networkThread;

	if (_sessionManager)
		delete _sessionManager;

	if (_ioMultiplexer)
		delete _ioMultiplexer;
}

bool Link::init(int maxSessions)
{
#ifdef WIN32
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
		return false;
#endif 

	_networkThread = new NetworkThread(*this);

	_ioMultiplexer = new IOMultiplexer();
	if (_ioMultiplexer->create(maxSessions) == false)
		return false;

	_sessionManager = new SessionManager(maxSessions);

	_acceptorManager = new AcceptorManager(_ioMultiplexer, _sessionManager);

	_initialized = true;

	return true;
}

void Link::close()
{
	if (_ioMultiplexer)
		_ioMultiplexer->close();

#ifdef WIN32
	WSACleanup();
#endif
}

void Link::run()
{
	_running = true;
	_networkThread->start();
}

void Link::stop()
{
	_running = false;
}

bool Link::isRunning()
{
	return _running;
}

bool Link::open(unsigned short port, Dispatcher* disp)
{
	if (_initialized == false) init();

	return this->open(nullptr, port, disp); 
}

bool Link::open(const char* ip, unsigned short port, Dispatcher* disp)
{
	if (_initialized == false) init();

	Acceptor* acceptor = _acceptorManager->create(disp);
	if (acceptor->open(ip, port) == false)
		return false;

	return true;
}

bool Link::connect(const char* ip, unsigned short port, Dispatcher* disp)
{
	if (_initialized == false) init();

	return true;
}

