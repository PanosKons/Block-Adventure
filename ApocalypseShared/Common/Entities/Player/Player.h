#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"
#include "Common/Blocks/Block.h"
#include "Common/Entities/Inventory/ItemStack.h"
#include "Common/World/WorldConstants.h"
#include "Common/Entities/Entity.h"
#include "Common/Entities/Credentials.h"

enum BlockInteractState
{
	Interact,
	StartedBreaking,
	EndedBreaking
};

class Player : public Entity
{
public:

	Player(Credentials& credentials);
	~Player();

	Block GetFacingBlock();
	Block GetBlockToPlace();
	int GetFirstAvaiableSlot(ItemStack itemStack);
	Vector3<double> GetLookPosition();

	Credentials credentials;

	char ActiveSlot;
	std::array<ItemStack, InventorySize> Inventory;

	float TimeToBreak = 0.0f;
	Vector3<int> BreakingBlockPosition;
	bool IsBreakingBlock = false;

	float BlockPlaceDelay = 0.3f;


	float JumpCooldown = 0.0f;
	bool Crouch = false;
	bool Godmode = true;

	//Field should be used only by the server
	bool IsReadyToReceivePackets = false;
};
