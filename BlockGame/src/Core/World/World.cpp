#include <Engine.h>
#include "../Blocks.h"
#include "World.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Math/EngineMath.h"
#include "Timer.h"
#include "SavingData.h"
void World::Save()
{
	for (auto& element : ChunkMap)
	{
		SavingData::SaveChunk(element.second);
	}
}
void World::Render()
{
	for (auto& element : ChunkMap)
	{
		if (element.second->Changed) {
			element.second->Draw();
			element.second->Changed = false;
		}
		Renderer::DrawChunk(element.second);
	}
	for (auto& element : ChunkMap)
	{
		Renderer::DrawChunkTransparent(element.second);
	}
}
Block* World::MakeBlock(BLOCK_ID id)
{
	switch (id)
	{
	case BLOCK_ID::Air:
		return new BlockAir;
		break;
	case BLOCK_ID::Cobblestone:
		return new BlockCobblestone;
		break;
	case BLOCK_ID::Grass:
		return new BlockGrass;
		break;
	case BLOCK_ID::Log:
		return new BlockLog;
		break;
	case BLOCK_ID::Iron:
		return new BlockIron;
		break;
	case BLOCK_ID::Dirt:
		return new BlockDirt;
	case BLOCK_ID::Glass:
		return new BlockGlass;
	case BLOCK_ID::Leaves:
		return new BlockLeaves;
	case BLOCK_ID::Water:
		return new BlockWater;
		break;
	}
	return nullptr;
}
static std::mutex s_ChunkMutex;
constexpr int temposize = 5;
void LoadChunk(Vector2<int> Position, World* world, std::unordered_map<int, Chunk*>* ChunkMap)
{
	Chunk* chunk = new Chunk({ Position.x,Position.y }, world);
	std::lock_guard<std::mutex> lock(s_ChunkMutex);
	ChunkMap->emplace(Position.y + Position.x * temposize, chunk);
}
World::World(int seed)
{
	Timer t;
	GameManager::Overworld = this;
	{
		std::array<std::future<void>, temposize* temposize> futures;
		for (int x = 0; x < temposize; x++)
		{
			for (int y = 0; y < temposize; y++)
			{
				futures[y + x * temposize] = std::async(std::launch::async, LoadChunk, Vector2<int>(x, y), this, &ChunkMap);
			}
		}
	}

	for (auto& element : ChunkMap)
	{
		element.second->UpdateAllBlocks();
	}
}
Block* World::GetBlock(Vector3<int> pos)
{
	if (pos.y < 0 || pos.y >= ChunkHeight || pos.z < 0 || pos.x < 0) return nullptr;
	int x = Math::Floor(pos.x / (float)ChunkSize);
	int z = Math::Floor(pos.z / (float)ChunkSize);
	if (ChunkMap.find(z + x * temposize) != ChunkMap.end())
		return ChunkMap[z + x * temposize]->GetBlock({ pos.x % ChunkSize, pos.y, pos.z % ChunkSize });
	return nullptr;
}
Chunk* World::GetChunk(Block* block)
{
	int x = block->Position.x / ChunkSize;
	int z = block->Position.z / ChunkSize;
	return ChunkMap[z + x * temposize];
}