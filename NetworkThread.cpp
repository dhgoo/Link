#include "Precompiled.h"
#include "NetworkThread.h"
#include "Link.h"
#include "EventObject.h"
#include "IOMultiplexer.h"

NetworkThread::NetworkThread(Link& link)
	: _link(link)
{
}

void* NetworkThread::process()
{
	while(_link._running)
	{
		_link._ioMultiplexer->waitForEvents();
	}

	return nullptr;
}
