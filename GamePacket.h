#pragma once

#include "PacketBase.h"

#pragma pack(push, 1)

namespace GamePacket
{
	enum PktID
	{
		LOGIN_REQ,
		LOGIN_RES,
		MAX,
	};

	struct LOGIN_REQ : public PacketBase
	{
		char userName[32];
	};

	struct LOGIN_RES : public PacketBase
	{
		int result;
	};
}

#pragma pack(pop)
