#include "pch.h"
#include "Player.h"
#include "Math/Ray.h"
#include "World/WorldConstants.h"
#include "Math/EngineMath.h"
#include "World/WorldManager.h"
Player::Player(Credentials& credentials)
	:credentials(credentials),IsReadyToReceivePackets(false)
{
	Position = { 20,60,260 };
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
}
Player::~Player() {}
Vector3<double> Player::GetLookPosition()
{
	return { Position.x,Position.y + 1.6, Position.z };
}
Block Player::GetFacingBlock()
{
	auto vec = GetLookPosition();
	Ray ray({vec.x, vec.y, vec.z}, Pitch, Yaw);
	Block block = WorldManager::BaseWorld->GetBlock({ Math::Floor(ray.getEnd().x), Math::Floor(ray.getEnd().y), Math::Floor(ray.getEnd().z) });
	while (true)
	{
		if (!block.IsValid()) break;
		if (block.GetBlockProperties().render) break;
		ray.step(0.1f);
		if (ray.getLength() > 5.9f)
			return Block();
		block = WorldManager::BaseWorld->GetBlock({ Math::Floor(ray.getEnd().x), Math::Floor(ray.getEnd().y), Math::Floor(ray.getEnd().z) });
	}
	return block;
}
Block Player::GetBlockToPlace()
{
	auto vec = GetLookPosition();
	Ray ray({ vec.x, vec.y, vec.z }, Pitch, Yaw);
	Block block = WorldManager::BaseWorld->GetBlock({ Math::Floor(ray.getEnd().x), Math::Floor(ray.getEnd().y), Math::Floor(ray.getEnd().z) });
	Block lastBlock = Block();
	while (true)
	{
		if (!block.IsValid()) break;
		if (block.GetBlockProperties().render) break;
		ray.step(0.1f);
		if (ray.getLength() > 5.9f)
			return Block();
		lastBlock = block;
		block = WorldManager::BaseWorld->GetBlock({ Math::Floor(ray.getEnd().x), Math::Floor(ray.getEnd().y), Math::Floor(ray.getEnd().z) });
	}
	return lastBlock;
}