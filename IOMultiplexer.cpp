#include "Precompiled.h"
#include "IOMultiplexer.h"
#include "Session.h"
#include "SessionManager.h"

IOMultiplexer::IOMultiplexer()
#ifdef WIN32
	: _hCompletionHandle(INVALID_HANDLE_VALUE)
#else
	: _epfd(-1)
	, _events(nullptr)
#endif
{
}

IOMultiplexer::~IOMultiplexer()
{
	close();
}

bool IOMultiplexer::create()
{
#ifdef WIN32
	_hCompletionHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	if (NULL == _hCompletionHandle)
	{
		printf("fail CreateIoCompletionPort(%u)", GetLastError());
		return false;
	}
#else
	_events = new epoll_event[MAX_EVENT];

	// Nowadays, size is ignored
	_epfd = epoll_create(maxevents);
	if (_epfd == -1)
	{
		printf("fail epoll_create() error(%d): %s\n", errno, strerror(errno));
		return false;
	}

	printf("epfd: %d\n", _epfd);
#endif
	return true;
}


void IOMultiplexer::close()
{
#ifdef WIN32
	if (_hCompletionHandle != INVALID_HANDLE_VALUE)
		CloseHandle(_hCompletionHandle);
#else
	::close(_epfd);

	if (_events != nullptr)
		delete[] _events;
#endif
}

#ifdef WIN32
bool IOMultiplexer::regist(int fd, uint32_t events, void* ptr)
{
	return true;
}
#else
bool IOMultiplexer::regist(int fd, uint32_t events, void* ptr)
{
	epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = events;
	ev.data.ptr = ptr;

	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev) != 0)
	{
		printf("fail epoll_ctl() _epfd(%d), fd(%d), ev.events(%u), ev.ptr(%p) error(%d): %s\n"
				, _epfd, fd, ev.events, ev.data.ptr, errno, strerror(errno));
		return false;
	}

	return true;
}
#endif

#ifdef WIN32
bool IOMultiplexer::unregist(int fd)
{
	return true;
}
#else
bool IOMultiplexer::unregist(int fd)
{
	epoll_event ev;
	memset(&ev, 0, sizeof(ev));

	if (epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &ev) != 0)
	{
		printf("fail epoll_ctl() _epfd(%d), fd(%d), error(%d): %s\n"
				, _epfd, fd, errno, strerror(errno));
		return false;
	}

	return true;
}
#endif

#ifdef WIN32
bool IOMultiplexer::waitForEvents()
{
	return true;
}
#else
bool IOMultiplexer::waitForEvents()
{
	int timeout = -1;
	int nfds = epoll_wait(_epfd, _events, MAX_EVENT, timeout);
	if (nfds == -1)
	{
		if (errno != EINTR)
		{
			printf("fail epoll_wait() error(%d): %s\n", errno, strerror(errno));
			return false;
		}
	}

	for (int i = 0; i < nfds; ++i)
	{
		EventObject *e = (EventObject* )_events[i].data.ptr;
		int events = _events[i].events;

		if (e->getType() == EVENT_OBJECT_ACCEPTOR)
		{
			if (events & EPOLLIN)
				e->onAccept();
			else if (events & (EPOLLERR | EPOLLHUP))
				e->onError(EVENT_ERROR_IO);
		}
		else if (e->getType() == EVENT_OBJECT_SESSION)
		{
			if (events & EPOLLIN)
				e->onRecv();
			else if (events & EPOLLOUT)
				e->onSend();
			else if (events & (EPOLLERR | EPOLLHUP))
				e->onError(EVENT_ERROR_IO);
		}
		else
		{
			assert(false);
		}
	}

	return true;		
}
#endif
