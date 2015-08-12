#pragma once

#include "StreamQueue.h"

class Stream;

typedef int (*handler)();

class Dispatcher
{
	typedef unordered_map<int, handler> PacketHandler;

protected:
	StreamQueue _recvQueue;
	StreamQueue _sendQueue;
	PacketHandler _packetHandler;

public:
	virtual bool parsing(Stream* raw);
	virtual void onConnect();
	virtual void onDisconnect(int err);
	virtual void registHandler();

	handler getHandler(int id);
};

