#include "World.h"
#include "GameManager.h"
#include "Renderer.h"
#include "../Math/EngineMath.h"
#include "../Timer.h"
void World::Render()
{
	for (Chunk* element : ChunkMap)
	{
		if (element->Changed) {
			element->Draw();
			element->Changed = false;
		}
		Renderer::DrawChunk(element);
	}
}
World::World(int seed)
{
	//Timer t;
	GameManager::Overworld = this;
	for (int x = 0; x < 6; x++)
	{
		for (int y = 0; y < 6; y++)
		{
			ChunkMap.push_back(new Chunk({ x,y }, this));
		}
	}
	for (Chunk* element : ChunkMap)
	{
		element->UpdateAllBlocks();
	}
}
Block* World::GetBlock(Vector3<int> pos) const
{
	if (pos.y < 0 || pos.y >= ChunkHeight) return nullptr;
	int x = Math::Floor(pos.x / (float)ChunkSize);
	int z = Math::Floor(pos.z / (float)ChunkSize);
	for (Chunk* const element : ChunkMap)
	{
		if (element->Position.x == x && element->Position.y == z) return element->GetBlock({ pos.x % ChunkSize, pos.y, pos.z % ChunkSize });
	}
	return nullptr;
}
Chunk* World::GetChunk(Block* block)
{
	for (Chunk* const element : ChunkMap)
	{
		if (element->Position.x == block->Transform.x / ChunkSize && element->Position.y == block->Transform.z / ChunkSize) return element;
	}
	return nullptr;
}