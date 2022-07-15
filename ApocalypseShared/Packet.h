#pragma once
#include <Engine.h>
#include "Math/Vector.h"
#include "BlockData.h"
constexpr int ChunkSize = 32;
constexpr int DefaultPacketSize = 128;
constexpr int ChunkPacketSize = ChunkSize * ChunkSize * ChunkSize * sizeof(BlockData);
constexpr int MAX_PLAYERS = 10;
enum class PACKET_ID
{
	//Sender: client and server
	//Data: char PlayerId
	//		Vector3<double> Position
	PlayerPosition,
	//Sender: client and server
	//Data: char PlayerId
	//		Vector3<int> BlockPosition
	//		int BLOCK_ID
	BreakBlock,
	//Sender: client
	//Data: Vector3<int> ChunkPosition
	RequestChunk,
	//Sender: server
	//Data: Vector3<int> ChunkPosition
	//		std::array<std::array<std::array<BlockData,ChunkSize>,ChunkSize>,ChunkSize> ChunkData
	NewChunk,
};
template<int TSize>
class Packet
{
public:
	Packet();
	Packet(const Packet& other) = delete;
	PACKET_ID GetPacketId();
	template<typename T>
	void AddPacketData(T Data);
	char* GetPacket();
	int GetPacketSize();
private:
	std::array<char, TSize> PacketData*;
	int LastIndex = 0;
};
template<int TSize>
template<typename T>
void Packet<TSize>::AddPacketData(T Data)
{
	*(T*)(PacketData->data()) = Data;
	LastIndex += sizeof(T);
}
template<int TSize>
Packet<TSize>::Packet(){}
template<int TSize>
PACKET_ID Packet<TSize>::GetPacketId()
{
	return *(PACKET_ID*)(this->PacketData);
}
template <int TSize>
char* Packet<TSize>::GetPacket()
{
	return (char*)PacketData;
}
template <int TSize>
int Packet<TSize>::GetPacketSize()
{
	return sizeof(PacketData);
}