#pragma once
#include "pch.h"
#include "Logger.h"
#include "Common/Math/Vector.h"
#include "Common/Blocks/Block.h"
#include "Common/World/WorldConstants.h"
#include "Common/Entities/Credentials.h"
#include "Common/Entities/Player/Player.h"

enum class Packet
{
	None,
	//ClientBound
	PlayerJoin, //Client
	ReplaceBlock, //Client
	NewChunk, //Client
	DeleteChunk, //Client
	HandleGui, //Client
	CreateEntity, //Client
	KillEntity, //Client

	//Mutual
	PlayerPosition, //Server, Client
	PlayerRotation, //Server, Client
	SelectSlot, //Server
	PlayerInventory,

	//ServerBound
	Command, // Server
	ActionPerformed // Server
};

constexpr int MAX_PLAYERS = 20;
struct SelectSlotData
{
	char ActiveSlot;
};
struct PlayerPositionData
{
	uint64_t UUID;
	Vector3<double> playerPosition;
};
struct EntityCreateData
{
	Entity entity;
};
struct EntityKillData
{
	uint64_t UUID;
};
struct PlayerRotationData
{
	uint64_t UUID;
	Vector2<float> playerRotation;
};
struct PlayerInventoryData
{
	uint64_t UUID;
	std::array<ItemStack, InventorySize> Inventory;
};
struct StartData
{
	Player player;
	int BlockCount;
	int ItemCount;
	int ModelCount;
	int InputActionCount;
};
struct ReplaceBlockData
{
	Vector3<int> Position;
	BlockType blockType;
};
struct CommandData
{
	Command command;
};
struct GuiData
{
	Gui gui;
	bool Open;
};
struct ActionPerformedData
{
	int identifier;
};