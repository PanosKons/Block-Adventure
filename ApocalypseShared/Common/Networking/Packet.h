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
	KeyPress,
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
	std::array<int, 8> WorldGen; // TEMPORARY
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
//constexpr int SizePacket = sizeof(PACKET_ID);
//
//constexpr int CredentialsPacketSize = sizeof(Credentials);
//constexpr int StartPacketSize = sizeof(Player) + sizeof(int) + sizeof(int) + 8 * sizeof(int);
//constexpr int BlockPropertiesSize = sizeof(BlockProperties);
//constexpr int ItemPropertiesSize = sizeof(ItemProperties);
//
//constexpr int ReceivePlayerPosition = sizeof(uint64_t) + sizeof(Vector3<double>);
//constexpr int SendPlayerPosition = SizePacket + ReceivePlayerPosition;
//
//constexpr int ReceivePlayerRotation = sizeof(uint64_t) + sizeof(Vector2<float>);
//constexpr int SendPlayerRotation = SizePacket + ReceivePlayerRotation;
//
//constexpr int ReceivePlayerJoin = sizeof(Player);
//constexpr int SendPlayerJoin = SizePacket + ReceivePlayerJoin;
//
//constexpr int ReceiveNewChunk = sizeof(Vector3<int>);
//constexpr int SendNewChunk = SizePacket + ReceiveNewChunk;
//constexpr int ChunkPacketSize = ChunkVolume * sizeof(BlockData);
//
//constexpr int ReceiveDeleteChunk = sizeof(Vector3<int>);
//constexpr int SendDeleteChunk = SizePacket + ReceiveDeleteChunk;
//
//constexpr int ReceiveReplaceBlock = sizeof(Vector3<int>) + sizeof(unsigned short);
//constexpr int SendReplaceBlock = ReceiveReplaceBlock + SizePacket;
//
//constexpr int ReceiveSelectSlot = sizeof(char);
//constexpr int SendSelectSlot = SizePacket + ReceiveSelectSlot;
//
//constexpr int ReceiveMouseState = 3 * sizeof(int);
//constexpr int SendMouseState = SizePacket + ReceiveMouseState;

//template<int TSize>
//class Packet
//{
//public:
//	Packet() {}
//	template<typename T>
//	const T& ExtractPacketData()
//	{
//		LastIndex += sizeof(T);
//		ASSERT((LastIndex <= GetPacketSize()), "Attempting to access fobidden memory!");
//		return *(T*)(this->PacketData->data() + LastIndex - sizeof(T));
//	}
//	template<typename T>
//	void AddPacketData(T Data)
//	{
//		if (PacketData == nullptr)
//			InitMemory();
//		ASSERT((LastIndex + sizeof(T) <= GetPacketSize()), "Attempting to write forbidden memory!");
//		*(T*)((char*)PacketData->data() + LastIndex) = Data;
//		LastIndex += sizeof(T);
//	}
//
//	char* GetPacket()
//	{
//		return PacketData->data();
//	}
//	int GetPacketSize()
//	{
//		return sizeof(*PacketData);
//	}
//
//	void SetPacket(std::array<char, TSize>* PacketData)
//	{
//		this->PacketData = PacketData;
//	}
//
//	void InitMemory()
//	{
//		PacketData = new std::array<char, TSize>();
//	}
//	~Packet()
//	{
//		//if(PacketData != nullptr)
//		//	delete PacketData;
//	}
//private:
//	std::array<char, TSize>* PacketData = nullptr;
//	int LastIndex = 0;
//};