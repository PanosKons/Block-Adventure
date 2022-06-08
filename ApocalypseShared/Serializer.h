#pragma once
constexpr int ChunkSize = 32;
constexpr int defaultsize = 200;
constexpr int MAX_PLAYERS = 10;
enum class PACKET_ID
{
	PlayerPosition, //Vector3<double>
	BreakBlock, //Vector3<int>, BLOCK_ID
	SendChunk, //Vector3<int>
	DeleteChunk, //Vector3<int>
};
class Serializer
{
	
};

