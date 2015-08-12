#pragma once

#include "EventObject.h"

class SessionManager;
class Dispatcher;
class Stream;

class Session : public EventObject
{
private:
	Dispatcher* _disp;
	Stream* _recvBuf;

public:
	Session(Dispatcher* disp);
	~Session();

	void onAccept();

	void onRecv();

	void onSend();

	void onError(int err);
};

