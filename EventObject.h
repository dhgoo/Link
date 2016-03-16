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
enum IoType
{
	IO_RECV,
	IO_SEND,	
};
#endif

class EventObject
#ifdef WIN32
	: public OVERLAPPED
#endif
{
protected:
	Socket _sock;
	int _type;
#ifdef WIN32
	int _ioType;
#endif

public:
	EventObject();
	virtual ~EventObject();

	Socket& socket();

	void setType(int type);
	int getType();

#ifdef WIN32
	void setIoType(int type);
	int getIoType();
#endif

	virtual void onAccept() {};
	virtual void onConnect() {};
	virtual void onRecv() {};
	virtual void onSend() {};
	virtual void onError(int err) = 0;
};

