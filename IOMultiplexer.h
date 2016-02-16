#pragma once

#ifdef WIN32

class IOMultiplexer
{
private:

public:
	IOMultiplexer();
	~IOMultiplexer();

	bool create(int maxevents);

	void close();

	bool regist(int fd, uint32_t events, void* ptr);

	bool unregist(int fd);

	bool waitForEvents();
};
#else
class IOMultiplexer
{
private:
	int _epfd;

	int _maxevents;

	epoll_event* _events;

public:
	IOMultiplexer();
	~IOMultiplexer();

	bool create(int maxevents);	
	
	void close();

	bool regist(int fd, uint32_t events, void* ptr);
	
	bool unregist(int fd);
	
	bool waitForEvents();
};
#endif
