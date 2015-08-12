#include "Precompiled.h"
#include "Thread.h"

Thread::Thread()
	: _threadid(0)
{
}

Thread::~Thread()
{
}

void* Thread::start_routine(void* arg)
{
	return ((Thread*)arg)->process();
}

bool Thread::run()
{
	int retval = pthread_create(&_threadid, nullptr, Thread::start_routine, this);
	if (retval != 0)
	{
		printf("fail pthread_create() error(%d): %s\n", retval, strerror(retval));
		return false;
	}

	return true;
}

void Thread::stop()
{
	int retval = pthread_join(_threadid, nullptr);
	if (retval != 0)
		printf("fail pthread_join() error(%d): %s\n", retval, strerror(retval));
}

