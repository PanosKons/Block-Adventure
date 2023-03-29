#pragma once
#include "pch.h"
#include "Player/Player.h"
#include "Common/World/WorldConstants.h"
#include "Common/Networking/Packet.h"
class EntityManager
{
public:
	static void Start();
	static void ShutDown();
	static Player* GetPlayer(uint64_t UUID);
	static bool CheckCollision(Vector3<double> Position, Vector3<double> Hitbox);
	inline static std::unordered_map<uint64_t,Player*> Players;
};

