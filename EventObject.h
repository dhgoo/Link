#pragma once

#include "Socket.h"

enum EventObjectType
{
	EVENT_OBJECT_TYPE_NONE,
	EVENT_OBJECT_ACCEPTOR,
	EVENT_OBJECT_CONNECTOR,
	EVENT_OBJECT_SESSION,
};

#ifdef WIN32
enum RequestEventType
{
	REQUEST_EVENT_SEND,
	REQUEST_EVENT_RECV,
};
#endif

class EventObject
{
protected:
#ifdef WIN32
	OVERLAPPED _overlapped;
	int _eventType;
#endif
	Socket _sock;

	int _type;	

public:
	EventObject();
	virtual ~EventObject();

	void setType(int type);
	int getType();

#ifdef WIN32
	void setEventType(int type);
	int getEventType();
#endif
	Socket& socket();

	virtual void onAccept() = 0;
	virtual void onRecv() = 0;
	virtual void onSend() = 0;
	virtual void onError(int err) = 0;
};

