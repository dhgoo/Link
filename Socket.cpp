#include "Precompiled.h"
#include "Socket.h"

Socket::Socket()
	: _fd(-1)
{
}

void Socket::setfd(int fd)
{
	_fd = fd;
}

int Socket::getfd()
{
	return _fd;
}

int Socket::create(int type)
{
	_fd = socket(AF_INET, type, 0);
	if (_fd == -1)
#ifdef WIN32
		printf("fail socket() err(%d)\n", WSAGetLastError());
#else
		printf("fail socket() err(%d) : %s\n", errno, strerror(errno));
#endif
	
	return _fd;
}

bool Socket::bind(const char* ip, unsigned short port)
{
	sockaddr_in in;
	in.sin_family = AF_INET;
	in.sin_port = htons(port);
	if (ip == nullptr)
		in.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		in.sin_addr.s_addr = inet_addr(ip);

	if (::bind(_fd, (sockaddr*)&in, sizeof(in)) != 0)
	{
		printf("fail bind() err(%d) : %s\n", errno, strerror(errno));
		return false;
	}

	return true;
}

bool Socket::listen()
{
	const int backlog = 5;
	if (::listen(_fd, backlog) != 0)
	{
		printf("fail listen() err(%d) : %s\n", errno, strerror(errno));
		return false;
	}

	return true;
}

int Socket::accept(sockaddr_in* addr)
{
	sockaddr_in in;
	socklen_t len = sizeof(in);
	int fd = ::accept(_fd, (sockaddr *)&in, &len);
	if (fd == -1)
	{
		printf("fail accept() error(%d): %s\n", errno, strerror(errno));
		return -1;
	}

	if (addr != nullptr)
		memcpy(addr, &in, sizeof(sockaddr));
	
	return fd;
}

int Socket::recv(void* buf, size_t count)
{
#ifdef WIN32
	int rbytes = ::recv(_fd, (char *)buf, count, 0);
#else
	ssize_t rbytes = read(_fd, buf, count);
#endif
	if (rbytes == -1)
	{
#ifdef WIN32
		if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAEINTR)
#else //WIN32
		if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
#endif //WIN32
			return 0;
		else
			return -1;
	}
	else if (rbytes == 0)
	{
		return -1;
	}

	return rbytes;
}

int Socket::send(const void* buf, size_t count)
{
#ifdef WIN32
	int sbytes = ::send(_fd, (char *)buf, count, 0);
#else
	ssize_t sbytes = write(_fd, buf, count);
#endif
	if (sbytes == -1)
	{
#ifdef WIN32
		if (WSAGetLastError() == WSAEWOULDBLOCK)
#else
		if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
#endif
			return 0;
		else
			return -1;
	}

	return sbytes;
}

bool Socket::nonblock()
{
#ifdef WIN32
	u_long flag = 1;
	ioctlsocket(_fd, FIONBIO, &flag);
#else
	int flags = fcntl(_fd, F_GETFL, 0);
	if (flags == -1)
	{
		printf("fail fcntl(F_GETFL) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
	
	if (fcntl(_fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		printf("fail fcntl(F_SETFL) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
#endif

	return true;
}
	
bool Socket::reuseaddrs()
{
	int optval = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval)) != 0)
	{
		printf("fail setsockopt(SO_REUSEADDR) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
	
	return true;
}

bool Socket::notimewait()
{
	struct linger l;
	l.l_onoff = 1;
	l.l_linger = 0;

#ifdef WIN32
	if (setsockopt(_fd, SOL_SOCKET, SO_LINGER, (const char*)&l, sizeof(l)) != 0)
#else
	if (setsockopt(_fd, SOL_SOCKET, SO_LINGER, (void*)&l, sizeof(l)) != 0)
#endif
	{
		printf("fail setsockopt(SO_LINGER) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
	
	return true;
}

bool Socket::nagleoff()
{
	int optval = 0;
#ifdef WIN32
	if (setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&optval, sizeof(optval)) != 0)
#else
	if (setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval)) != 0)
#endif
	{
		printf("fail setsockopt(TCP_NODELAY) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
	
	return true;
}

int Socket::getSendBufSize()
{
	int bufsize = 0;
#ifdef WIN32
	if (setsockopt(_fd, SOL_SOCKET, SO_SNDBUF, (const char*)&bufsize, sizeof(bufsize)) != 0)
#else //WIN32
	if (setsockopt(_fd, SOL_SOCKET, SO_SNDBUF, (void*)&bufsize, sizeof(bufsize)) != 0)
#endif //WIN32
	{
		printf("fail setsockopt(SO_SNDBUF) err(%d) : %s\n", errno, strerror(errno));
		return -1;
	}
	
	return bufsize;
}

int Socket::getRecvBufSize()
{
	int bufsize = 0;
#ifdef WIN32
	if (setsockopt(_fd, SOL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(bufsize)) != 0)
#else //WIN32
	if (setsockopt(_fd, SOL_SOCKET, SO_RCVBUF, (void*)&bufsize, sizeof(bufsize)) != 0)
#endif //WIN32
	{
		printf("fail setsockopt(SO_REVBUF) err(%d) : %s\n", errno, strerror(errno));
		return -1;
	}
	
	return bufsize;
}

bool Socket::setSendBufSize(int size)
{
	int bufsize = size;
#ifdef WIN32
	if (setsockopt(_fd, SOL_SOCKET, SO_SNDBUF, (const char*)&bufsize, sizeof(bufsize)) != 0)
#else //WIN32
	if (setsockopt(_fd, SOL_SOCKET, SO_SNDBUF, (void*)&bufsize, sizeof(bufsize)) != 0)
#endif //WIN32
	{
		printf("fail setsockopt(SO_SNDBUF) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
	
	return true;
}

bool Socket::setRecvBufSize(int size)
{
	int bufsize = size;
#ifdef WIN32
	if (setsockopt(_fd, SOL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(bufsize)) != 0)
#else //WIN32
	if (setsockopt(_fd, SOL_SOCKET, SO_RCVBUF, (void*)&bufsize, sizeof(bufsize)) != 0)
#endif //WIN32
	{
		printf("fail setsockopt(SO_REVBUF) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
	
	return true;
}



