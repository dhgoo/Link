#pragma once

class Session;
class Dispatcher;

class SessionManager
{
	typedef unordered_set<Session* > Sessions;

private:
	Sessions _activatedSessions;

public:
	SessionManager();
	~SessionManager();

	Session* create(Dispatcher* disp);
	void destroy(Session* s);
};

