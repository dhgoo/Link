#pragma once

#include "Thread.h"

class Link;

class NetworkThread : public Thread
{
private:
	Link& _link;

public:
	NetworkThread(Link& link);

	void* process();
};

