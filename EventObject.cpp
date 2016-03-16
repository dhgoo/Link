#include "Precompiled.h"
#include "EventObject.h"

EventObject::EventObject()
	: _type(EVENT_OBJECT_TYPE_NONE)
#ifdef WIN32
	, _ioType(IO_RECV)
#endif
{
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

#ifdef WIN32
void EventObject::setIoType(int type)
{
	_ioType = type;
}

int EventObject::getIoType()
{
	return _ioType;
}
#endif

Socket& EventObject::socket()
{
	return _sock;
}

