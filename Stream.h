#pragma once

class Stream
{
private:
	char* _buf;
	char* _readptr;
	char* _writeptr; 
	char* _startptr;
	char* _endptr;
	int _capacity;

public:
	Stream();
	~Stream();

	void initialize(char* buf, int len);
	int capacity();
	int length();
	int remain();

	char* readptr();
	char* writeptr();
	char* startptr();
	char* endptr();

	void read(int byte);
	void write(int byte);
	void arrange();

	Stream* split(int len);
	
	static Stream* create(int len);
	static void destroy(Stream* stream);
};

