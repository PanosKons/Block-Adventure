#include "pch.h"
#include "Player.h"
#include "Math/Ray.h"
#include "World/WorldConstants.h"
#include "Math/EngineMath.h"
#include "World/WorldManager.h"

Player::Player(Credentials& credentials)
	:credentials(credentials),IsReadyToReceivePackets(false)
{
	Position = { 2600,80,2600 };
	Hitbox = { 0.6, 1.8 ,0.6 };
	Velocity = { 0,0,0 };
	MaxHealth = 100.0f;
	Health = 100.0f;
	Speed = 20.0f;
	Pitch = -24.0f;
	Yaw = -1.0f;
	Grounded = false;
	ActiveSlot = 0;
	Inventory = {};
	BlockPlaceDelay = 0.3f;
	JumpCooldown = 0.0f;
	Crouch = false;
	Godmode = true;
	ReachDistance = 6;
	cameraMode = CameraMode::FirstPerson;
}
Player::~Player() {}
Vector3<double> Player::GetLookPosition()
{
	switch (cameraMode)
	{
	case CameraMode::FirstPerson:
	{
		return { Position.x ,Position.y + 1.6 , Position.z };
		break;
	}
	case CameraMode::ThirdPersonBack:
	{
		Vector3<double> Direction = { cos(Math::Radians(Yaw)),tan(Math::Radians(Pitch)),sin(Math::Radians(Yaw)) };
		Direction /= Direction.Magnitude();
		Direction *= 2;
		return { Position.x - Direction.x ,Position.y + 1.6 - Direction.y, Position.z - Direction.z };
		break;
	}
	case CameraMode::ThirdPersonFront:
	{
		Vector3<double> Direction = { cos(Math::Radians(Yaw)),tan(Math::Radians(Pitch)),sin(Math::Radians(Yaw)) };
		Direction /= Direction.Magnitude();
		Direction *= 4;
		return { Position.x + Direction.x ,Position.y + 1.6 + Direction.y, Position.z + Direction.z };
		break;
	}
	}

	ERR("Player::GetLookPosition");
	return Vector3<double>();
}
Entity* Player::GetFacingEntity()
{
	Ray ray(GetLookPosition(), Pitch, Yaw);
	std::vector<Entity*> ReachableEntities;
	Vector3<int> ChunkPosition = Vector::IntVector(Position) / ChunkSize;
	for (int x = ChunkPosition.x - 1; x <= ChunkPosition.x + 1; x++)
	{
		for (int y = ChunkPosition.y - 1; y <= ChunkPosition.y + 1; y++)
		{
			for (int z = ChunkPosition.z - 1; z <= ChunkPosition.z + 1; z++)
			{
				Chunk* chunk = WorldManager::BaseWorld->GetChunkDirect({ x,y,z });
				if (chunk == nullptr) continue;
				for (Entity& entity : chunk->entities)
				{
					if (Math::Powd(entity.Position.x - Position.x, 2) + Math::Powd(entity.Position.y - Position.y, 2) + Math::Powd(entity.Position.z - Position.z, 2) <= Math::Powi((int)ReachDistance, 2))
					{
						ReachableEntities.push_back(&entity);
					}
				}
			}
		}
	}
	while (true)
	{
		for (Entity* entity : ReachableEntities)
			if (ray.CurrentPosition.x >= entity->Position.x - entity->Hitbox.x / 2
				&& ray.CurrentPosition.x <= entity->Position.x + entity->Hitbox.x / 2
				&& ray.CurrentPosition.y >= entity->Position.y - entity->Hitbox.y / 2
				&& ray.CurrentPosition.y <= entity->Position.y + entity->Hitbox.y / 2
				&& ray.CurrentPosition.z >= entity->Position.z - entity->Hitbox.z / 2
				&& ray.CurrentPosition.z <= entity->Position.z + entity->Hitbox.z / 2) return entity;

		ray.Step(0.05);
		if (ray.GetLengthSquared() > ReachDistance * ReachDistance)
			return nullptr;
	}
}
Block Player::GetFacingBlock()
{
	Ray ray(GetLookPosition(), Pitch, Yaw);
	Block block = WorldManager::BaseWorld->GetBlock(Vector::FloorVector(ray.CurrentPosition));
	while (true)
	{
		if (!block.IsValid()) break;
		if (block.GetBlockProperties().render)
		{
			for (Face& face : block.GetBlockModel().Faces)
			{
				Vector3<double> direction = ray.GetDirection();
				if (face.direction == Direction::Forward || face.direction == Direction::Backward)
				{
					double t = ((double)(face.position.z + block.Position.z) - ray.LastPosition.z) / direction.z;
					Vector3<double> point = ray.LastPosition + ray.GetDirection() * t;
					if (point.x - block.Position.x >= face.position.x && point.x - block.Position.x <= face.position.x + face.size.x && point.y - block.Position.y >= face.position.y && point.y - block.Position.y <= face.position.y + face.size.y) return block;
				}
				if (face.direction == Direction::Right || face.direction == Direction::Left)
				{
					double t = ((double)(face.position.x + block.Position.x) - ray.LastPosition.x) / direction.x;
					Vector3<double> point = ray.LastPosition + ray.GetDirection() * t;
					if (point.z - block.Position.z >= face.position.z && point.z - block.Position.z <= face.position.z + face.size.x && point.y - block.Position.y >= face.position.y && point.y - block.Position.y <= face.position.y + face.size.y) return block;
				}
				if (face.direction == Direction::Up || face.direction == Direction::Down)
				{
					
					double t = ((double)(face.position.y + block.Position.y) - ray.LastPosition.y) / direction.y;
					Vector3<double> point = ray.LastPosition + ray.GetDirection() * t;
					if (point.x - block.Position.x >= face.position.x && point.x - block.Position.x <= face.position.x + face.size.x && point.z - block.Position.z >= face.position.z && point.z - block.Position.z <= face.position.z + face.size.y) return block;
				}
			}
		}
		ray.Step(0.05);
		if (ray.GetLengthSquared() > ReachDistance * ReachDistance)
			return Block();
		block = WorldManager::BaseWorld->GetBlock(Vector::FloorVector(ray.CurrentPosition));
	}
	return block;
}
Block Player::GetBlockToPlace()
{
	Ray ray(GetLookPosition(), Pitch, Yaw);
	Block block = WorldManager::BaseWorld->GetBlock(Vector::FloorVector(ray.CurrentPosition));
	Block lastBlock = Block();
	while (true)
	{
		if (!block.IsValid()) break;
		if (block.GetBlockProperties().render)
		{
			for (Face& face : block.GetBlockModel().Faces)
			{
				Vector3<double> direction = ray.GetDirection();
				if (face.direction == Direction::Forward || face.direction == Direction::Backward)
				{
					double t = ((double)(face.position.z + block.Position.z) - ray.LastPosition.z) / direction.z;
					Vector3<double> point = ray.LastPosition + ray.GetDirection() * t;
					if (point.x - block.Position.x >= face.position.x && point.x - block.Position.x <= face.position.x + face.size.x && point.y - block.Position.y >= face.position.y && point.y - block.Position.y <= face.position.y + face.size.y) return lastBlock;
				}
				if (face.direction == Direction::Right || face.direction == Direction::Left)
				{
					double t = ((double)(face.position.x + block.Position.x) - ray.LastPosition.x) / direction.x;
					Vector3<double> point = ray.LastPosition + ray.GetDirection() * t;
					if (point.z - block.Position.z >= face.position.z && point.z - block.Position.z <= face.position.z + face.size.x && point.y - block.Position.y >= face.position.y && point.y - block.Position.y <= face.position.y + face.size.y) return lastBlock;
				}
				if (face.direction == Direction::Up || face.direction == Direction::Down)
				{

					double t = ((double)(face.position.y + block.Position.y) - ray.LastPosition.y) / direction.y;
					Vector3<double> point = ray.LastPosition + ray.GetDirection() * t;
					if (point.x - block.Position.x >= face.position.x && point.x - block.Position.x <= face.position.x + face.size.x && point.z - block.Position.z >= face.position.z && point.z - block.Position.z <= face.position.z + face.size.y) return lastBlock;
				}
			}
		}
		ray.Step(0.05);
		if (ray.GetLengthSquared() > ReachDistance * ReachDistance)
			return Block();
		lastBlock = block;
		block = WorldManager::BaseWorld->GetBlock(Vector::FloorVector(ray.CurrentPosition));
	}
	return lastBlock;
}