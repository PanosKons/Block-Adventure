#pragma once
#include "Common\Entities\EntityManager.h"
#include "Entities/PlayerClient.h"
class EntityManagerClient : public EntityManager
{
public:
	static void Tick();
	static PlayerClient& GetPlayer();
	static void CreateSelf(int PlayerId, Player* player);
	static void CreatePlayer(int PlayerId);
};

