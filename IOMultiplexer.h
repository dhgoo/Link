#pragma once

class IOMultiplexer
{
private:
	int _epfd;

	int _maxevents;

	epoll_event* _events;

public:
	IOMultiplexer();

	bool create(int maxevents);	
	
	void close();

	bool regist(int fd, uint32_t events, void* ptr);
	
	bool unregist(int fd);
	
	bool waitForEvents();
};

