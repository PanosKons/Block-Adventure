#pragma once
#include "pch.h"
typedef std::array<char, 8> GameName;
class Credentials
{
public:
	static uint64_t ConvertToUUID(GameName Name)
	{
		return *((uint64_t*)Name.data());
	}
	Credentials(GameName Name, int UUID) : Name(Name), UUID(UUID) {}
	GameName Name;
	uint64_t UUID;
};
std::ostream& operator<<(std::ostream& os, GameName const& m);