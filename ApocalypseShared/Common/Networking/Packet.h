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

	//Mutual
	PlayerPosition, //Server, Client
	PlayerRotation, //Server, Client
	SelectSlot, //Server
	PlayerInventory,

	//ServerBound
	MouseState, //Server
	KeyPress, //Server
	Command, // Server
};

constexpr int MAX_PLAYERS = 20;
struct MouseStateData
{
	MouseState LeftMouse, RightMouse, MiddleMouse;
};
struct SelectSlotData
{
	char ActiveSlot;
};
struct PlayerPositionData
{
	uint64_t UUID;
	Vector3<double> playerPosition;
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
};
struct ReplaceBlockData
{
	Vector3<int> Position;
	BlockType blockType;
};
struct KeyData
{
	bool PKeyPressed = false;
	bool RKeyPressed = false;
};
struct CommandData
{
	Command command;
};