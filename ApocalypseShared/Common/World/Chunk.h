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

	void Refresh();
	void RefreshBorders();

	Block GetBlock(Vector3<int> RelativePosition) const;
	Vector3<int> GetPosition() const { return Position; };
	BlockArray* GetBlocks() { return blocks; }
	bool MeshChanged = true;
	bool DontRender = true;
private:
	World* world;
	Vector3<int> Position;
	BlockArray* blocks;
};