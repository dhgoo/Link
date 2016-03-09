#pragma once

#define MAX_EVENT 10240

class IOMultiplexer
{
private:

#ifdef WIN32
	HANDLE _hCompletionHandle;
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
	bool regist(int fd, uint32_t events, void* ptr);
	bool unregist(int fd);
	bool waitForEvents();
#else
	bool regist(int fd, uint32_t events, void* ptr);	
	bool unregist(int fd);	
	bool waitForEvents();
#endif
};

