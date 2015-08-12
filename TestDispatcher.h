#pragma once

#include "Dispatcher.h"

class TestDispatcher : public Dispatcher
{
public:
	virtual void registHandler();
	virtual bool parsing(Stream* buf);
	virtual void onConnect();
	virtual void onDisconnect(int err);
	virtual void onSend();
	virtual void onRecv();

	int LOGIN_RES();
};

