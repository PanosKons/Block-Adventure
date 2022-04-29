#include <Engine.h>
#include "../Blocks.h"
#include "World.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Math/EngineMath.h"
#include "Timer.h"
#include "SavingData.h"
int64_t ToLong(int x, int y)
{
	int64_t data = 0;
	int* p = (int*)&data;
	(*p) = x;
	p++;
	(*p) = y;
	return data;
}
Vector2<int> ToVector(int64_t value)
{
	Vector2<int> v;
	int* p = (int*)&value;
	v.x = *p;
	p++;
	v.y = *p;
	return v;
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
constexpr int spawnChunks = 3;
void LoadChunk(Vector2<int> Position, World* world, std::unordered_map<int64_t, Chunk*>* ChunkMap)
{
	static std::mutex s_ChunkMutex;
	Chunk* chunk = new Chunk({ Position.x,Position.y }, world);
	std::lock_guard<std::mutex> lock(s_ChunkMutex);
	ChunkMap->emplace(ToLong(Position.x,Position.y), chunk);
}
World::World(int seed)
{
	GameManager::Overworld = this;
	//{
	//	std::array<std::future<void>, spawnChunks * spawnChunks> futures;
	//	for (int x = 0; x < spawnChunks; x++)
	//	{
	//		for (int y = 0; y < spawnChunks; y++)
	//		{
	//			futures[y + x * spawnChunks] = std::async(std::launch::async, LoadChunk, Vector2<int>(x, y), this, &ChunkMap);
	//		}
	//	}
	//}

	//for (auto& element : ChunkMap)
	//{
	//	element.second->UpdateAllBlocks();
	//}
}
Block* World::GetBlock(Vector3<int> pos)
{
	if (pos.y < 0 || pos.y >= ChunkHeight) return nullptr;
	int x = Math::Floor(pos.x / (float)ChunkSize);
	int z = Math::Floor(pos.z / (float)ChunkSize);
	if (ChunkMap.find(ToLong(x,z)) != ChunkMap.end())
		return ChunkMap[ToLong(x, z)]->GetBlock({ (pos.x + BIG_NUMBER) % ChunkSize, pos.y, (pos.z + BIG_NUMBER) % ChunkSize });
	return nullptr;
}
Chunk* World::GetChunk(Block* block)
{
	int x = block->Position.x / ChunkSize;
	int z = block->Position.z / ChunkSize;
	return ChunkMap[ToLong(x, z)];
}
void World::LoadNewChunks(Vector2<int> position)
{
	if (ChunkMap.find(ToLong(position.x, position.y)) == ChunkMap.end())
	{
		LoadChunk(Vector2<int>(position.x, position.y), this, &ChunkMap);
		ChunkMap[ToLong(position.x, position.y)]->UpdateAllBlocks();
		if (ChunkMap.find(ToLong(position.x + 1, position.y)) != ChunkMap.end())
			ChunkMap[ToLong(position.x + 1, position.y)]->UpdateBorderBlocks();
		if (ChunkMap.find(ToLong(position.x - 1, position.y)) != ChunkMap.end())
			ChunkMap[ToLong(position.x - 1, position.y)]->UpdateBorderBlocks();
		if (ChunkMap.find(ToLong(position.x, position.y + 1)) != ChunkMap.end())
			ChunkMap[ToLong(position.x, position.y + 1)]->UpdateBorderBlocks();
		if (ChunkMap.find(ToLong(position.x, position.y - 1)) != ChunkMap.end())
			ChunkMap[ToLong(position.x, position.y - 1)]->UpdateBorderBlocks();
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
	auto ChunkMapCopy = ChunkMap;
	for (auto& [value, chunk] : ChunkMapCopy)
	{
		Vector2<int> pos = ToVector(value);
		if (Math::Abs(pos.x - position.x) > radius || Math::Abs(pos.y - position.y) > radius)
		{
			delete chunk;
			ChunkMap.erase(value);
		}
	}
}