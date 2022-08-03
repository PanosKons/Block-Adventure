#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"
#include "Common/Blocks/Block.h"
#include "Common/World/WorldConstants.h"

constexpr int StartPacketSize = 1000;
constexpr int DefaultPacketSize = 128;
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
	//		std::array<std::array<std::array<BlockData,ChunkSize>,ChunkSize>,ChunkSize> ChunkData
	NewChunk,
};
template<int TSize>
class Packet
{
public:
	Packet();
	template<typename T>
	const T& ExtractPacketData();
	template<typename T>
	void AddPacketData(T Data);

	char* GetPacket();
	int GetPacketSize();

	void SetPacket(std::array<char, TSize>* PacketData);

	void InitMemory();
private:
	std::array<char, TSize>* PacketData;
	int LastIndex = 0;
};
template<int TSize>
template<typename T>
void Packet<TSize>::AddPacketData(T Data)
{
	if (PacketData == null)
		InitMemory();
	*(T*)(PacketData->data()) = Data;
	LastIndex += sizeof(T);
}
template<int TSize>
Packet<TSize>::Packet(){}
template<int TSize>
void Packet<TSize>::InitMemory()
{
	PacketData = new std::array<char, TSize>();
}
template<int TSize>
template<typename T>
const T& Packet<TSize>::ExtractPacketData()
{
	LastIndex += sizeof(T);
	return (T*)(this->PacketData + LastIndex - sizeof(T));
}
template <int TSize>
char* Packet<TSize>::GetPacket()
{
	return PacketData.data();
}
template <int TSize>
int Packet<TSize>::GetPacketSize()
{
	return sizeof(PacketData);
}
template<int TSize>
void Packet<TSize>::SetPacket(std::array<char, TSize>* PacketData)
{
	this->PacketData = PacketData;
}
