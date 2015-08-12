#pragma once

#pragma pack(push, 1)

struct PacketBase
{
	int length;
	unsigned short id;

	int getLength() { return length; }
	unsigned short getId() { return id; }
};

#pragma pack(pop)
