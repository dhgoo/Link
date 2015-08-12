#include "Precompiled.h"
#include "TestDispatcher.h"
#include "GamePacket.h"

void TestDispatcher::registHandler()
{
}

bool TestDispatcher::parsing(Stream* buf)
{
	return true;
}

void TestDispatcher::onConnect()
{
}

void TestDispatcher::onDisconnect(int err)
{
}

void TestDispatcher::onSend()
{
}

void TestDispatcher::onRecv()
{
}

int TestDispatcher::LOGIN_RES()
{
	return 0;
}

