#pragma once

class Session;
class Dispatcher;

class SessionManager
{
	typedef unordered_set<Session* > Sessions;

private:
	Sessions _activatedSessions;
	int _maxSessions;

public:
	SessionManager(int maxSessions);
	~SessionManager();

	Session* create(Dispatcher* disp);
	void destroy(Session* s);
};

