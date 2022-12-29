#pragma once
#include "Common/World/World.h"
#include "Common/Entities/Player/Player.h"

class Reader
{
public:
	static void Init();
	static Player* ReadPlayer(Credentials& credentials);
	static void WriteAllPlayers();
	static BlockArray* ReadWorldChunk(World* world,Vector3<int> ChunkPosition);
	static void WriteWorld(World* world);
};

