#include "Precompiled.h"
#include "Thread.h"

Thread::Thread()
	: _threadID(0)
#ifdef _WIN32
	, _hThread(INVALID_HANDLE_VALUE)
#endif //WIN32
{
}

Thread::~Thread()
{
	_threadID = 0;
#ifdef WIN32
	CloseHandle(_hThread);
	_hThread = INVALID_HANDLE_VALUE;
#endif //WIN32
}

#ifdef WIN32
unsigned int WINAPI Thread::start_routine(void* arg)
#else //WIN32
void* Thread::start_routine(void* arg)
#endif //WIN32
{
	Thread* inst = (Thread*)arg;
	inst->process();
	return 0;
}

bool Thread::start()
{
#ifdef WIN32
	_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread::start_routine, this, 0, (unsigned int *)&_threadID);
	if (_hThread == 0)
	{
		printf("fail _beginthreadex() error(%d)\n", errno);
		return false;
	}
#else //WIN32
	int retval = pthread_create(&_threadID, nullptr, Thread::start_routine, this);
	if (retval != 0)
	{
		printf("fail pthread_create() error(%d): %s\n", retval, strerror(retval));
		return false;
	}
#endif //WIN32
	return true;
}

void Thread::stop()
{
#ifdef WIN32
	if (WaitForSingleObject(_hThread, INFINITE) == WAIT_FAILED)
		printf("fail WaitForSingleObject() error(%d)\n", GetLastError());
#else //WIN32
	int retval = pthread_join(_threadID, nullptr);
	if (retval != 0)
		printf("fail pthread_join() error(%d): %s\n", retval, strerror(retval));
#endif //WIN32
}

