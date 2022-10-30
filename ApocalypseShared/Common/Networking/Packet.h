#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"
#include "Common/Blocks/Block.h"
#include "Common/World/WorldConstants.h"

constexpr int StartPacketSize = 1000;
constexpr int DefaultPacketSize = 64;
constexpr int ChunkPacketSize = ChunkVolume * sizeof(BlockData);
constexpr int MAX_PLAYERS = 20;

enum class PACKET_ID
{
	//Sender: server
	//Data: char PlayerId
	//		Vector3<double> Position
	PlayerJoin,
	//Sender: server
	//Data: char PlayerId
	//		Vector3<double> Position
	//Sender: client
	//Data: Vector3<double> Position
	PlayerPosition,
	//Sender: server
	//Data: char PlayerId
	//		Vector3<int> BlockPosition
	//		int BLOCK_ID
	//Sender: client
	//Data: Vector3<int> BlockPosition
	//		int BLOCK_ID
	BreakBlock,
	//Sender: client
	//Data: Vector3<int> ChunkPosition
	RequestChunk,
	//Sender: server
	//Data: Vector3<int> ChunkPosition
	//		BlockArray ChunkData
	NewChunk,
};
template<int TSize>
class Packet
{
public:
	Packet() {}
	template<typename T>
	const T& ExtractPacketData()
	{
		LastIndex += sizeof(T);
		return *(T*)(this->PacketData->data() + LastIndex - sizeof(T));
	}
	template<typename T>
	void AddPacketData(T Data)
	{
		if (PacketData == nullptr)
			InitMemory();
		*(T*)((char*)PacketData->data() + LastIndex) = Data;
		LastIndex += sizeof(T);
	}

	char* GetPacket()
	{
		return PacketData->data();
	}
	int GetPacketSize()
	{
		return sizeof(*PacketData);
	}

	void SetPacket(std::array<char, TSize>* PacketData)
	{
		this->PacketData = PacketData;
	}

	void InitMemory()
	{
		PacketData = new std::array<char, TSize>();
	}
	void DeletePacket()
	{
		delete PacketData;
	}
private:
	std::array<char, TSize>* PacketData = nullptr;
	int LastIndex = 0;
};