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
	void LoadNewChunk(Vector3<int> ChunkPosition);
	void LoadPlayerChunks(Vector3<int> ChunkPosition,int RenderDistance);
	void UnLoadPlayerChunks(Vector3<int> ChunkPosition, int RenderDistance);
private:
	std::unordered_map<int64_t, Chunk*> ChunkMap;
};
