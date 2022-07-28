#pragma once
#include "pch.h"
#include "Entity.h"
#include "Common/World/WorldConstants.h"
class EntityManager
{
public:
	static void Start();
	static void ShutDown();
	static void CreatePlayer(int PlayerId);
	static Entity* GetEntity(int PlayerId);
protected:
	inline static std::vector<Entity*> Players;
};

