#include "Precompiled.h"
#include "Acceptor.h"
#include "IOMultiplexer.h"
#include "Session.h"
#include "SessionManager.h"

Acceptor::Acceptor(IOMultiplexer* ioMux, SessionManager* ssMgr, Dispatcher* disp)
	: _ioMux(ioMux)
	, _ssMgr(ssMgr)
	, _disp(disp)
{
}

bool Acceptor::open(unsigned short port)
{
	return open(nullptr, port);
}

bool Acceptor::open(const char* ip, unsigned short port)
{
	if (_sock.create(SOCK_STREAM) == -1)
		return false;

	if (_sock.bind(ip, port) == false)
		return false;

	if (_sock.nonblock() == false)
		return false;

	if (_sock.reuseaddrs() == false)
		return false;

	if (_sock.listen() == false)
		return false;

#ifdef WIN32
#else
	uint32_t events = EPOLLIN | EPOLLHUP | EPOLLERR;
	if (_ioMux->regist(_sock.getfd(), events, this) == false)
		return false;
#endif

	return true;
}

void Acceptor::onAccept()
{
	sockaddr_in addr;
	int fd = _sock.accept(&addr);
	if (fd == -1)
	{
		printf("fail accept()\n");
		return;
	}

	Session* s = _ssMgr->create(_disp);

	s->socket().setfd(fd);

#ifdef WIN32
#else
	uint32_t events = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR;
	if (_ioMux->regist(s->socket().getfd(), events, s) == false)
	{
		printf("cannot regist event for new session\n");
		return;
	}
#endif

	printf( "ip = %s  port = %d\n", inet_ntoa(addr.sin_addr),  ntohs((short)addr.sin_port));
}

void Acceptor::onRecv()
{
	// can't reach
}

void Acceptor::onSend()
{
	// can't reach
}

void Acceptor::onError(int err)
{

}

