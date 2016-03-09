#include "Precompiled.h"
#include "EventObject.h"

EventObject::EventObject()
	: _type(EVENT_OBJECT_TYPE_NONE)
{
#ifdef WIN32
	memset(&_overlapped, 0, sizeof(_overlapped));
#endif
}

EventObject::~EventObject()
{
}

void EventObject::setType(int type)
{
	_type = type;
}

int EventObject::getType()
{
	return _type;
}

Socket& EventObject::socket()
{
	return _sock;
}

