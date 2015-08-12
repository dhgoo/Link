#include "Precompiled.h"
#include "AcceptorManager.h"
#include "IOMultiplexer.h"
#include "SessionManager.h"
#include "Dispatcher.h"
#include "Acceptor.h"

AcceptorManager::AcceptorManager(IOMultiplexer* ioMux, SessionManager* ssMgr)
	: _ioMux(ioMux)
	, _ssMgr(ssMgr)
{
}

AcceptorManager::~AcceptorManager()
{
	for (auto itr = _activatedAcceptors.begin(); itr != _activatedAcceptors.end(); ++itr)
	{
		SAFE_DELETE(*itr);
	}
}

Acceptor* AcceptorManager::create(Dispatcher* disp)
{
	Acceptor* acceptor = new Acceptor(_ioMux, _ssMgr, disp);

	acceptor->setType(EVENT_OBJECT_ACCEPTOR);

	_activatedAcceptors.push_back(acceptor);

	return acceptor;
}

