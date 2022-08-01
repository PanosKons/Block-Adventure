#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"
#include "Common/Blocks/Block.h"
#include "Common/Entities/Inventory/ItemStack.h"
#include "Common/World/WorldConstants.h"
#include "Common/Entities/Entity.h"

class Player : public Entity
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

	char ActiveSlot;
	std::array<ItemStack, InventorySize> Inventory;

	float TimeToBreak = 0.0f;
	Vector3<int> BreakingBlockPosition;
	bool IsBreakingBlock = false;

	float BlockPlaceDelay = 0.3f;


	float JumpCooldown = 0.0f;
	bool Crouch = false;
	bool Godmode = true;

};
