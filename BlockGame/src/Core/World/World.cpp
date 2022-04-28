#include <Engine.h>
#include "../Blocks.h"
#include "World.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Math/EngineMath.h"
#include "Timer.h"
#include "SavingData.h"
int64_t Vector2ToLong(int x, int y)
{
	int64_t data = 0;
	int* p = (int*)&data;
	(*p) = x;
	p++;
	(*p) = y;
	return data;
}
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
	case BLOCK_ID::Cobblestone:
		return new BlockCobblestone;
	case BLOCK_ID::Grass:
		return new BlockGrass;
	case BLOCK_ID::Log:
		return new BlockLog;
	case BLOCK_ID::Iron:
		return new BlockIron;
	case BLOCK_ID::Dirt:
		return new BlockDirt;
	case BLOCK_ID::Glass:
		return new BlockGlass;
	case BLOCK_ID::Leaves:
		return new BlockLeaves;
	case BLOCK_ID::Water:
		return new BlockWater;
	case BLOCK_ID::DryGrass:
		return new BlockDryGrass;
	}
	return nullptr;
}
static std::mutex s_ChunkMutex;
constexpr int temposize = 6;
void LoadChunk(Vector2<int> Position, World* world, std::unordered_map<int64_t, Chunk*>* ChunkMap)
{
	Chunk* chunk = new Chunk({ Position.x,Position.y }, world);
	std::lock_guard<std::mutex> lock(s_ChunkMutex);
	ChunkMap->emplace(Position.y + Position.x * temposize, chunk);
	//ChunkMap->emplace(Vector2ToLong(Position.x,Position.y), chunk);
}
World::World(int seed)
{
	Timer t;
	GameManager::Overworld = this;
	{
		std::array<std::future<void>, temposize* temposize> futures;
		for (int x = 0; x < temposize-1; x++)
		{
			for (int y = 0; y < temposize-1; y++)
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
	//if (ChunkMap.find(Vector2ToLong(pos.x,pos.z)) != ChunkMap.end())
	//	return ChunkMap[Vector2ToLong(pos.x, pos.z)]->GetBlock({ pos.x % ChunkSize, pos.y, pos.z % ChunkSize });
	return nullptr;
}
Chunk* World::GetChunk(Block* block)
{
	int x = block->Position.x / ChunkSize;
	int z = block->Position.z / ChunkSize;
	return ChunkMap[z + x * temposize];
	/*return ChunkMap[Vector2ToLong(x, z)];*/
}
void World::LoadNewChunks(Vector2<int> position)
{
	if (ChunkMap.find(position.y + position.x * temposize) == ChunkMap.end())
	{
		LoadChunk(Vector2<int>(position.x, position.y), this, &ChunkMap);
		ChunkMap[position.y + position.x * temposize]->UpdateAllBlocks();
	}
}
void World::LoadNewChunks(Vector2<int> position,int radius)
{
 	int startX = position.x - radius;
	int startY = position.y - radius;
	int EndX = position.x + radius;
	int EndY = position.y + radius;
	for (int x = startX; x <= EndX; x++)
	{
		for (int y = startY; y <= EndY; y++)
		{
			LoadNewChunks({ x, y });
		}
	}
}