#include "Precompiled.h"
#include "StreamQueue.h"
#include "Stream.h"

StreamQueue::StreamQueue()
{
}

StreamQueue::~StreamQueue()
{
}

Stream* StreamQueue::front()
{
	return _streamQueue.front();
}

void StreamQueue::push_front(Stream* s)
{
	_streamQueue.push_front(s);
}

void StreamQueue::push_back(Stream* s)
{
	_streamQueue.push_back(s);
}

void StreamQueue::pop_front()
{
	_streamQueue.pop_front();
}

