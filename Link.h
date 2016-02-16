#pragma once

class IOMultiplexer;
class SessionManager;
class AcceptorManager;
class NetworkThread;
class Dispatcher;
class EventObject;
class Acceptor;
class Session;

class Link 
{
	friend class NetworkThread;

private:
	IOMultiplexer*	_ioMultiplexer;
	SessionManager* _sessionManager;
	NetworkThread* _networkThread;
	AcceptorManager* _acceptorManager;
	bool _initialized;
	bool _running;

public:
	Link();
	~Link();

	void run();

	void stop();

	bool isRunning();
	
	bool open(unsigned short port, Dispatcher* disp);

	bool open(const char* ip, unsigned short port, Dispatcher* disp);

	bool connect(const char* ip, unsigned short port, Dispatcher* disp);

private:
	bool init(int maxSessions = MAX_SESSIONS);

	void close();
};

