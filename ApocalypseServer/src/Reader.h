#pragma once
#include "Common/World/World.h"
#include "Common/Entities/Player/Player.h"

class Reader
{
public:
	static void Init();
	static Player* ReadPlayer(int PlayerId);
	static void WritePlayer(Player* Player);
	static void ReadWorld(World* world);
	static void WriteWorld(World* world);
};

