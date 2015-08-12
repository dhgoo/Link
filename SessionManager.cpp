#include "Precompiled.h"
#include "SessionManager.h"
#include "Session.h"
#include "IOMultiplexer.h"

SessionManager::SessionManager(int maxsessions)
	: _maxSessions(maxsessions)
{
}

SessionManager::~SessionManager()
{
}

Session* SessionManager::create(Dispatcher* disp)
{
	Session* s = new Session(disp);
	
	s->setType(EVENT_OBJECT_SESSION);

	_activatedSessions.insert(s);
	
	return s;
}

void SessionManager::destroy(Session* s)
{
	if (s == nullptr)
		return;

	_activatedSessions.erase(s);

	SAFE_DELETE(s);
}

