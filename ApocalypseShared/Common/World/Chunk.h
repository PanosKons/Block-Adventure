#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"
#include "Structure.h"
#include "Common/Blocks/Block.h"
#include "Common/Networking/Packet.h"
class World;
class Chunk {
public:

	Chunk(Vector3<int> ChunkPosition,World* world);
	~Chunk();

	Block GetBlock(Vector3<int> RelativePosition) const;
	Vector3<int> GetPosition() const;

	void SpawnStructure(Vector3<int> RelativePosition, std::string&& name);
	void UpdateAllBlocks();
	void UpdateBorderBlocks();
	bool Changed = true;
	bool ShouldUpdate = true;
	bool ShouldUpdateBorders = true;
private:
	World* world;
	Vector3<int> Position;
	std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks;
};