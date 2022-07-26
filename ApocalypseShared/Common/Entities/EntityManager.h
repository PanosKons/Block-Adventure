#pragma once
#include "pch.h"
#include "Entity.h"
#include "Common/World/WorldConstants.h"
class EntityManager
{
public:
	inline static std::vector<Entity*> Players;

	static void Start();
	static void ShutDown();
	static void CreatePlayer(int PlayerId);
};

