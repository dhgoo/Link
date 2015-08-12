#include "Precompiled.h"
#include "Dispatcher.h"
#include "Stream.h"
#include "PacketBase.h"

bool Dispatcher::parsing(Stream* raw)
{
	while (raw->length() >= (int)sizeof(PacketBase))
	{
		PacketBase* header = reinterpret_cast<PacketBase* >(raw->readptr());

		if (header->getLength() >= raw->length())
		{
			Stream* s = raw->split(header->getLength());
			if (s == nullptr)
			{
				return false;
			}

			// need to thread sync

			_recvQueue.push_back(s);
		}
		else
		{
			break;
		}
	}

	return true;
}

void Dispatcher::onConnect()
{
}

void Dispatcher::onDisconnect(int err)
{
}

void Dispatcher::registHandler()
{
}

handler Dispatcher::getHandler(int id)
{
	auto itr = _packetHandler.find(id);
	if (itr == _packetHandler.end())
		return nullptr;

	return itr->second;
}
