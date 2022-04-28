#pragma once
#include "../Math/Vector.h"
#include <memory>
#include <array>
#include <list>
#include <tuple>
#include "../vendor/hash_for_tuples.h"
#include "Block.h"
#include "Chunk.h"
class World
{
public:
	World(int seed);
	void Render();
	Block* GetBlock(Vector3<int> pos) const;
	Chunk* GetChunk(Block* block);
private:
	std::list<Chunk*> ChunkMap;
};
