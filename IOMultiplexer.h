#pragma once

#define MAX_EVENT 10240

class IOMultiplexer
{
private:

#ifdef WIN32
	HANDLE _iocp;
#else
	int _epfd;
	int _maxevents;
	epoll_event* _events;
#endif

public:
	IOMultiplexer();
	~IOMultiplexer();

	bool create();

	void close();

#ifdef WIN32
	bool regist(SOCKET s);
	bool unregist(int fd);
	bool waitForEvents(unsigned long* transferred, unsigned long* key, LPOVERLAPPED* overlapped);
#else
	bool regist(int fd, uint32_t events, void* ptr);	
	bool unregist(int fd);	
	bool waitForEvents();
#endif
};

