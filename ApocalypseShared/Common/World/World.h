#pragma once
#include "pch.h"
#include "Math/Vector.h"
#include "../Blocks/Block.h"
#include "Chunk.h"
#include "WorldConstants.h"

typedef std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize> BlockArray;

class World
{
public:

	World();

	Chunk* GetChunk(Vector3<int> AbsolutePosition) const;
	Block GetBlock(Vector3<int> AbsolutePosition) const;

	void CreateChunk(Vector3<int> ChunkPosition, BlockArray* blocks);
	void DestroyChunk(Vector3<int> ChunkPosition);
private:
	std::unordered_map<int64_t, Chunk*> ChunkMap;
};
