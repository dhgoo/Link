#pragma once

#ifdef WIN32
#include <process.h>
#else //WIN32
#include <pthread.h>
#endif //WIN32

class Thread
{
protected:
#ifdef WIN32
	HANDLE _hThread;
	unsigned int _threadID;
#else //WIN32
	pthread_t _threadID;
#endif //WIN32

public:
	Thread();
	virtual ~Thread();
	
	bool start();
	void stop();


protected:

#ifdef _WIN32
	static unsigned int WINAPI start_routine(void* arg);
#else //WIN32
	static void* start_routine(void* arg);
#endif //WIN32

	virtual void* process() = 0;
};
