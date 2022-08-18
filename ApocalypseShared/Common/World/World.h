#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"
#include "Common/Blocks/Block.h"
#include "Chunk.h"
#include "WorldConstants.h"

class World
{
public:

	World();

	Chunk* GetChunkAbsolute(Vector3<int> AbsolutePosition) const;
	Chunk* GetChunkDirect(Vector3<int> ChunkPosition) const;
	Block GetBlock(Vector3<int> AbsolutePosition) const;

	void CreateChunk(Vector3<int> ChunkPosition, BlockArray* blocks);
	void DestroyChunk(Vector3<int> ChunkPosition);

	std::unordered_map<int64_t, Chunk*> const * GetChunkMap() { return &ChunkMap; }
private:
	std::unordered_map<int64_t, Chunk*> ChunkMap;
};
