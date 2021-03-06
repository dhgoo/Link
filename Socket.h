#pragma once

class Socket
{
private:
	SOCKET _fd;

public:
	Socket();
	~Socket();

	// 
	void setfd(int fd);

	// return socket file descriptor
	int getfd();

	// 
	void setSocket(SOCKET s);

	//
	int getSocket();

	// create socket. using SOCK_STREAM or SOCK_DGRAM for type
	int create(int type);

	// 
	bool bind(const char* ip, unsigned short port);

	//
	bool listen();

	//
	bool connect(const char* ip, unsigned short port);

	// 
	int accept(sockaddr_in* in);

	//
	int recv(void* buf, size_t count);

	//
	int send(const void* buf, size_t count);

	// change to non blocking socket
	bool nonblock();

	// set reuse address 
	bool reuseaddrs();

	// disable TIME-WAIT
	bool notimewait();

	// nagle algorithm off
	bool nagleoff();

#ifdef WIN32
	bool inheritProperties(SOCKET parent);
#endif

	// 
	int getSendBufSize();

	//
	int getRecvBufSize();

	// 
	bool setSendBufSize(int size);

	// 
	bool setRecvBufSize(int size);
};

