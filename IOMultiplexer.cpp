#include "Precompiled.h"
#include "IOMultiplexer.h"
#include "Session.h"
#include "SessionManager.h"

#ifdef WIN32
IOMultiplexer::IOMultiplexer()
	: _maxevents(0)
	, _events(nullptr)
{
}

IOMultiplexer::~IOMultiplexer()
{
	close();
}

bool IOMultiplexer::create(int maxevents)
{
	_maxevents = maxevents;

	_events = new WSAEVENT[maxevents];

	return true;
}

void IOMultiplexer::close()
{
	if (_events != nullptr)
		delete[] _events;
}

bool IOMultiplexer::regist(SOCKET fd, uint32_t events, void* ptr)
{
	if (WSAEventSelect(fd, , events) == SOCKET_ERROR)
	{
		printf("fail epoll_ctl() _epfd(%d), fd(%d), ev.events(%u), ev.ptr(%p) error(%d): %s\n"
			, _epfd, fd, ev.events, ev.data.ptr, errno, strerror(errno));
	}


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

bool IOMultiplexer::waitForEvents()
{
	int timeout = -1;
	int nfds = epoll_wait(_epfd, _events, _maxevents, timeout);
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
		EventObject *e = (EventObject*)_events[i].data.ptr;
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
#else
IOMultiplexer::IOMultiplexer()
	: _epfd(-1)
	, _maxevents(0)
	, _events(nullptr)
{
}

IOMultiplexer::~IOMultiplexer()
{
	close();
}

bool IOMultiplexer::create(int maxevents)
{
	_maxevents = maxevents;

	_events = new epoll_event[maxevents];

	// Nowadays, size is ignored
	_epfd = epoll_create(maxevents);
	if (_epfd == -1)
	{
		printf("fail epoll_create() error(%d): %s\n", errno, strerror(errno));
		return false;
	}

	printf("epfd: %d\n", _epfd);

	return true;
}

void IOMultiplexer::close()
{
	::close(_epfd);

	if (_events != nullptr)
		delete[] _events;
}

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

bool IOMultiplexer::waitForEvents()
{
	int timeout = -1;
	int nfds = epoll_wait(_epfd, _events, _maxevents, timeout);
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


