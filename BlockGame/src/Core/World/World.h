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
	Block* GetBlock(Vector3<int> pos);
	Chunk* GetChunk(Block* block);
	static Block* MakeBlock(BLOCK_ID id);
	void Save();
	void LoadNewChunks(Vector2<int> position);
private:
	std::unordered_map<int64_t, Chunk*> ChunkMap;
};
