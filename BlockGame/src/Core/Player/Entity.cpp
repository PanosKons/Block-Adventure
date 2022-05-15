#include "Entity.h"
#include "EntityManager.h"
Entity::Entity()
{
	EntityManager::Entities.push_back(this);
}