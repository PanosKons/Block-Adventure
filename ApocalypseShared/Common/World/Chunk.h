#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"
#include "Structure.h"
#include "Common/Blocks/Block.h"
#include "Common/Networking/Packet.h"

typedef std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize> BlockArray;
class World;

class Chunk {
public:

	Chunk(Vector3<int> ChunkPosition,World* world, BlockArray* blocks);
	~Chunk();

	Block GetBlock(Vector3<int> RelativePosition) const;
	Vector3<int> GetPosition() const { return Position; };

	//void SpawnStructure(Vector3<int> RelativePosition, std::string&& name);
	void UpdateAllBlocks();
	void UpdateBorderBlocks();
	BlockArray* GetBlocks() { return blocks; }

	bool Changed = true;
	bool ShouldUpdate = true;
	bool ShouldUpdateBorders = true;
private:
	World* world;
	Vector3<int> Position;
	BlockArray* blocks;
};