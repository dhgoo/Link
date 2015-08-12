#pragma once

class Socket
{
private:
	int _fd;

public:
	Socket();

	// 
	void setfd(int fd);

	// return socket file descriptor
	int getfd();

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
	bool reuse();

	// 
	bool disableTimeWait();

	// 
	bool nagleOff();

	// 
	int getSendBufSize();

	//
	int getRecvBufSize();

	// 
	bool setSendBufSize(int size);

	// 
	bool setRecvBufSize(int size);
};

