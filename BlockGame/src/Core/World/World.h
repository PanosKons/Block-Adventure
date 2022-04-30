#pragma once
#include <Engine.h>
#include "Math/Vector.h"
#include "../Blocks/Block.h"
#include "Chunk.h"
class World
{
public:
	World(int seed);
	void Render();
	Block* GetBlock(Vector3<int> AbsolutePosition) const;
	Chunk* GetChunk(Vector3<int> AbsolutePosition) const;
	static Block* MakeBlock(BLOCK_ID id);
	void Save();
	void LoadNewChunks(Vector2<int> ChunkPosition);
	void LoadNewChunks(Vector2<int> ChunkPosition,int radius);
private:
	std::unordered_map<int64_t, Chunk*> ChunkMap;
};
