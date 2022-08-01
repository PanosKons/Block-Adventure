#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"
#include "Common/Blocks/Block.h"
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

	std::unordered_map<int64_t, Chunk*>* GetChunkMap() { return &ChunkMap; }
private:
	std::unordered_map<int64_t, Chunk*> ChunkMap;
};
