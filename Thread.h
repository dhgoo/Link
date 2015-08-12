#pragma once

#ifdef WIN32
#include 
#else //WIN32
#include <pthread.h>
#endif //WIN32

class Thread
{
protected:
	pthread_t _threadid;

public:
	Thread();
	virtual ~Thread();
	
	bool run();

	void stop();

protected:
	static void* start_routine(void* arg);

	virtual void* process() = 0;
};
