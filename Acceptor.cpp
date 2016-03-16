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
	if (_ioMux->regist(_sock.getfd()) == false)
		return false;

	if (waitForAccept() == false)
		return false;
#else
	uint32_t events = EPOLLIN | EPOLLHUP | EPOLLERR;
	if (_ioMux->regist(_sock.getfd(), events, this) == false)
		return false;
#endif

	return true;
}

#ifdef WIN32
bool Acceptor::waitForAccept()
{
	// OVERLAPPED 구조체를 초기화 시켜주지 않으면, IOCP가 제대로 동작하지 않는다
	OVERLAPPED* p = static_cast<OVERLAPPED*>(this);
	memset(p, 0, sizeof(OVERLAPPED));
	memset(&_acceptBuffer, 0, sizeof(_acceptBuffer));
	unsigned long received = 0;

	_acceptSocket.create(SOCK_STREAM);

	if (INVALID_SOCKET == AcceptEx(_sock.getSocket(), _acceptSocket.getSocket(), _acceptBuffer, MAX_ACCEPT_BUFFER - ((sizeof(sockaddr_in)+16)*2),
								sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16, &received, (LPOVERLAPPED)this))
		return false;

	return true;
}
#endif

#ifdef WIN32
void Acceptor::onAccept()
{
	_acceptSocket.inheritProperties(_sock.getSocket());

	sockaddr_in addr;
	int len = sizeof(addr);
	getpeername(_acceptSocket.getSocket(), (sockaddr*)&addr, &len);

	Session* session = _ssMgr->create(_disp);

	session->socket().setSocket(_acceptSocket.getSocket());

	_ioMux->regist(_acceptSocket.getSocket());

	waitForAccept();

	printf("ip = %s  port = %d\n", inet_ntoa(addr.sin_addr), ntohs((short)addr.sin_port));
}
#else
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

	printf( "ip = %s  port = %d\n", inet_ntoa(addr.sin_addr),  ntohs((short)addr.sin_port));
}
#endif

void Acceptor::onError(int err)
{

}

