#include "Precompiled.h"
#include "Stream.h"

Stream::Stream()
	: _buf(nullptr)
	, _readptr(nullptr)
	, _writeptr(nullptr)
	, _startptr(nullptr)
	, _endptr(nullptr)
	, _capacity(0) 
{
}

Stream::~Stream()
{
}

void Stream::initialize(char* buf, int len)
{
	_buf = buf;
	_writeptr = _buf;
	_readptr = _buf;
	_startptr = _buf;
	_endptr = _buf + len;
	_capacity = len;
}

int Stream::capacity()
{
	return _capacity;
}

int Stream::length()
{
	return (int)(_writeptr - _readptr);
}

int Stream::remain()
{
	return (int)(_endptr - _writeptr);
}

char* Stream::readptr()
{
	return _readptr;
}

char* Stream::writeptr()
{
	return _writeptr;
}

char* Stream::startptr()
{
	return _startptr;
}

char* Stream::endptr()
{
	return _endptr;
}

void Stream::read(int byte)
{
	_readptr += byte;

	if (_readptr > _writeptr)
	{
		assert(false);
	}
}

void Stream::write(int byte)
{
	_writeptr += byte;

	if (_writeptr > _endptr)
	{
		assert(false);
	}
}

void Stream::arrange()
{
	int len = length();
	if (len == 0)
	{
		_readptr = _startptr;
		_writeptr = _startptr;
	}
	else
	{
		memmove(_startptr, _readptr, len);
		_readptr = _startptr;
		_writeptr = _startptr + len;
	}
}

Stream* Stream::split(int len)
{
	if (len > length())
	{
		return nullptr;
	}

	Stream* s = create(len);
	
	if (!s)
	{
		return nullptr;
	}

	memcpy(s->writeptr(), this->readptr(), len);
	
	s->write(len);

	this->read(len);

	return s;
}

Stream* Stream::create(int len)
{
	char* buf = new char[len];
	Stream* stream = new Stream;

	stream->initialize(buf, len);

	return stream;
}

void Stream::destroy(Stream* stream)
{
	if (stream == nullptr)
		return;
	
	char* buffer = stream->startptr();

	SAFE_DELETE(buffer);

	SAFE_DELETE(stream);
}

