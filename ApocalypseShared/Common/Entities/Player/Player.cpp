#include "pch.h"
#include "Player.h"
#include "Math/Ray.h"
#include "World/WorldConstants.h"
#include "Math/EngineMath.h"
#include "World/WorldManager.h"
Player::Player(Credentials& credentials)
	:credentials(credentials)
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
	TimeToBreak = 0.0f;
	BreakingBlockPosition = { 0,0,0 };
	IsBreakingBlock = false;
	BlockPlaceDelay = 0.3f;
	JumpCooldown = 0.0f;
	Crouch = false;
	Godmode = true;
}
Player::~Player() {}

int Player::GetFirstAvaiableSlot(int id,TYPE type)
{
	for (unsigned int i = 0; i < Inventory.size(); i++)
	{
		if (Inventory[i].id == id && Inventory[i].type == type) return i;
	}
	for (unsigned int i = 0; i < Inventory.size(); i++)
	{
		if (Inventory[i].count == 0) return i;
	}
	return -1;
}
Vector3<double> Player::GetLookPosition()
{
	return { Position.x,Position.y + 1.6, Position.z };
}
Block Player::GetFacingBlock()
{
	auto vec = GetLookPosition();
	Ray ray({vec.x, vec.y, vec.z}, Pitch, Yaw);
	Block block = WorldManager::BaseWorld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	while (true)
	{
		if (block.data == nullptr) break;
		if (block.GetBlockId() != BLOCK_ID::Air) break;
		ray.step(0.1f);
		if (ray.getLength() > 5.9f)
			return Block();
		block = WorldManager::BaseWorld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	}
	return block;
}
Block Player::GetBlockToPlace()
{
	auto vec = GetLookPosition();
	Ray ray({ vec.x, vec.y, vec.z }, Pitch, Yaw);
	Block block = WorldManager::BaseWorld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	Block lastBlock = Block();
	while (true)
	{
		if (block.data == nullptr) break;
		if (block.GetBlockId() != BLOCK_ID::Air) break;
		ray.step(0.1f);
		if (ray.getLength() > 5.9f)
			return Block();
		lastBlock = block;
		block = WorldManager::BaseWorld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	}
	return lastBlock;
}
void Player::MarkBlockToBreak()
{
	Block block = GetFacingBlock();
	if (block.data == nullptr) return;
	IsBreakingBlock = true;
	BreakingBlockPosition = block.Position;
	TimeToBreak = (float)block.GetBlockProperties().hardness;
}