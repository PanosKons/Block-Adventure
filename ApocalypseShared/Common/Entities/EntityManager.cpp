#include "pch.h"
#include "EntityManager.h"
#include "Common/Networking/Packet.h"
#include "Common/World/WorldManager.h"
#include "Common/Math/EngineMath.h"

void EntityManager::Start(){}
bool EntityManager::CheckCollision(Vector3<double> Position, Vector3<double> Hitbox)
{
	Vector3<int> Point1 = { Math::Floor(Position.x - Hitbox.x / 2), Math::Floor(Position.y), Math::Floor(Position.z - Hitbox.z / 2) };
	Vector3<int> Point2 = { Math::Floor(Position.x + Hitbox.x / 2), Math::Floor(Position.y + Hitbox.y), Math::Floor(Position.z + Hitbox.z / 2) };
	double PlayerY = Position.y + Hitbox.y / 2;
	for (int x = Point1.x; x <= Point2.x; x++)
	{
		for (int y = Point1.y; y <= Point2.y; y++)
		{
			for (int z = Point1.z; z <= Point2.z; z++)
			{
				Block block = WorldManager::BaseWorld->GetBlock({ x,y,z });
				if(block.IsValid())
				{
					if (block.GetBlockProperties().render)
					{
						for (Face& face : block.GetBlockModel().Faces)
						{
							if (face.direction == Direction::Forward || face.direction == Direction::Backward)
							{
								if (
									(Math::Abs(Position.x - (face.position.x + block.Position.x + face.size.x / 2)) < (face.size.x + Hitbox.x) / 2) &&
									(Math::Abs(PlayerY - (face.position.y + block.Position.y + face.size.y / 2)) < (face.size.y + Hitbox.y) / 2) &&
									(Math::Abs(Position.z - (face.position.z + block.Position.z)) < Hitbox.z / 2)
									) return true;
							}
							else if (face.direction == Direction::Right || face.direction == Direction::Left)
							{
								if (
									(Math::Abs(Position.x - (face.position.x + block.Position.x)) < Hitbox.x / 2) &&
									(Math::Abs(PlayerY - (face.position.y + block.Position.y + face.size.y / 2)) < (face.size.y + Hitbox.y) / 2) &&
									(Math::Abs(Position.z - (face.position.z + block.Position.z + face.size.x / 2)) < (face.size.x + Hitbox.z) / 2)
									) return true;
							}
							else if (face.direction == Direction::Up || face.direction == Direction::Down)
							{
								if (
									(Math::Abs(Position.x - (face.position.x + block.Position.x + face.size.x / 2)) < (face.size.x + Hitbox.x) / 2) &&
									(Math::Abs(PlayerY - (face.position.y + block.Position.y)) < Hitbox.y / 2) &&
									(Math::Abs(Position.z - (face.position.z + block.Position.z + face.size.y / 2)) < (face.size.y + Hitbox.z) / 2)
									) return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}
void EntityManager::ShutDown()
{
	for (auto[UUID,player] : Players)
	{
		delete player;
	}
}
Player* EntityManager::GetPlayer(uint64_t UUID)
{
	return Players[UUID];
}
void EntityManager::CreateEntity(Entity& entity)
{
	Chunk* chunk = WorldManager::BaseWorld->GetChunkAbsolute(Vector::IntVector(entity.Position));
	if (chunk != nullptr)
		chunk->entities.push_back(entity);
}

void EntityManager::KillEntity(uint64_t UUID)
{
	for (auto& [key, chunk] : *WorldManager::BaseWorld->GetChunkMap())
	{
		for (int i = 0; i < chunk->entities.size(); i++)
		{
			if (chunk->entities[i].UUID == UUID)
			{
				chunk->entities.erase(chunk->entities.begin() + i);
				return;
			}
		}
	}
}

Entity* EntityManager::GetEntity(uint64_t UUID)
{
	for (auto& [key, chunk] : *WorldManager::BaseWorld->GetChunkMap())
	{
		for (int i = 0; i < chunk->entities.size(); i++)
		{
			if (chunk->entities[i].UUID == UUID)
			{
				return &chunk->entities[i];
			}
		}
	}
	return nullptr;
}
