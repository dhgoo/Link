#pragma once

class Stream;

class StreamQueue
{
private:
	list<Stream* > _streamQueue;

public:
	StreamQueue();
	~StreamQueue();

	Stream* front();
	
	void pop_front();

	void push_front(Stream* s);

	void push_back(Stream* s);
};

