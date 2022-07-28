#pragma once
#include "pch.h"
#include "Math/Vector.h"
#include "Blocks/Block.h"
#include "ItemStack.h"
#include "World/WorldConstants.h"
class Player
{
public:

	Player();
	void Update();
	~Player();

	Block GetFacingBlock();
	Block GetBlockToPlace();
	void MarkBlockToBreak();
	int GetFirstAvaiableSlot(int id, TYPE type);
	Vector3<double> GetLookPosition();

	Vector3<double> Position;
	Vector3<double> Velocity;
	Vector3<double> Hitbox;
	float Yaw;
	float Pitch;

	float MaxHealth;
	float Health;
	float Speed;

	char ActiveSlot;
	std::array<ItemStack, InventorySize> Inventory;

	float TimeToBreak = 0.0f;
	Block BreakingBlock;
	bool IsBreakingBlock = false;


	float JumpCooldown = 0.0f;
	bool Crouch = false;
	bool Grounded = false;

	bool Godmode = true;

};
