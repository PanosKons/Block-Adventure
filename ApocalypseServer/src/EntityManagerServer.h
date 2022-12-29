#pragma once
#include "Common/Entities/EntityManager.h"
class EntityManagerServer : public EntityManager
{
public:
	static void CreatePlayer(Credentials& credentials);
};

