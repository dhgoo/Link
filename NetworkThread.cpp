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
#ifdef WIN32
	LPOVERLAPPED	overlapped	= nullptr;
	unsigned long	transferred	= 0;
	unsigned long	key			= 0;
	bool			result		= false;
#endif

	while(_link._running)
	{
#ifdef WIN32
		overlapped = nullptr; transferred = 0; key = 0;

		result = _link._ioMultiplexer->waitForEvents(&transferred, &key, &overlapped);
		if (result == false && overlapped == nullptr)
		{
			// TODO print critical log
			continue;
		}

		if (key == 0)
		{
			// TODO print log
		}

		Overlapped* pDispatch = (Overlapped*)overlapped;

		SAFE_DELETE(pDispatch);
#else
		_link._ioMultiplexer->waitForEvents();
#endif
	}

	return nullptr;
}
