#pragma once

#include "Socket.h"

enum EventObjectType
{
	EVENT_OBJECT_TYPE_NONE,
	EVENT_OBJECT_ACCEPTOR,
	EVENT_OBJECT_CONNECTOR,
	EVENT_OBJECT_SESSION,
};

class EventObject
{
protected:
	int _type;
	Socket _sock;

public:
	EventObject();
	virtual ~EventObject();

	void setType(int type);
	int getType();

	Socket& socket();

	virtual void onAccept() = 0;
	virtual void onRecv() = 0;
	virtual void onSend() = 0;
	virtual void onError(int err) = 0;
};

