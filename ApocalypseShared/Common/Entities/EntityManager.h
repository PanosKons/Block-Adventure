#pragma once
#include "pch.h"
#include "Player/Player.h"
#include "Common/World/WorldConstants.h"
class EntityManager
{
public:
	static void Start();
	static void ShutDown();
	static Player* GetPlayer(int PlayerId);
	static bool CheckCollision(Vector3<double> Position, Vector3<double> Hitbox);
	static bool CheckCollision(Vector3<double> Position, Vector3<double> Hitbox, Vector3<int> block);
	inline static std::vector<Player*> Players;
};

