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
Block* World::MakeBlock(BLOCK_ID id) //Has nothing to do with world
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
World::World(int seed)
{
	GameManager::Overworld = this;
}
Block* World::GetBlock(Vector3<int> pos) const
{
	if (pos.y < 0 || pos.y >= ChunkHeight) return nullptr;
	int x = Math::Floor(pos.x / (float)ChunkSize);
	int z = Math::Floor(pos.z / (float)ChunkSize);
	auto it = ChunkMap.find(ToLong(x, z));
	if (it != ChunkMap.end())
		return it->second->GetBlock({ (pos.x + BIG_NUMBER) % ChunkSize, pos.y, (pos.z + BIG_NUMBER) % ChunkSize });
	return nullptr;
}
Chunk* World::GetChunk(Vector3<int> Position) const
{
	int x = Position.x / ChunkSize;
	int z = Position.z / ChunkSize;
	auto it = ChunkMap.find(ToLong(x, z));
	if(it != ChunkMap.end())
		return it->second;
	return nullptr;
}
void World::LoadNewChunk(Vector2<int> ChunkPosition)
{
	if (ChunkMap.find(ToLong(ChunkPosition.x, ChunkPosition.y)) == ChunkMap.end())
	{
		Chunk* chunk = new Chunk({ ChunkPosition.x,ChunkPosition.y }, this);
		ChunkMap.emplace(ToLong(ChunkPosition.x, ChunkPosition.y), chunk);
		ChunkMap[ToLong(ChunkPosition.x, ChunkPosition.y)]->UpdateAllBlocks();
		if (ChunkMap.find(ToLong(ChunkPosition.x + 1, ChunkPosition.y)) != ChunkMap.end())
			ChunkMap[ToLong(ChunkPosition.x + 1, ChunkPosition.y)]->UpdateBorderBlocks();
		if (ChunkMap.find(ToLong(ChunkPosition.x - 1, ChunkPosition.y)) != ChunkMap.end())
			ChunkMap[ToLong(ChunkPosition.x - 1, ChunkPosition.y)]->UpdateBorderBlocks();
		if (ChunkMap.find(ToLong(ChunkPosition.x, ChunkPosition.y + 1)) != ChunkMap.end())
			ChunkMap[ToLong(ChunkPosition.x, ChunkPosition.y + 1)]->UpdateBorderBlocks();
		if (ChunkMap.find(ToLong(ChunkPosition.x, ChunkPosition.y - 1)) != ChunkMap.end())
			ChunkMap[ToLong(ChunkPosition.x, ChunkPosition.y - 1)]->UpdateBorderBlocks();
	}
}
void World::LoadPlayerChunks(Vector2<int> ChunkPosition,int RenderDistance)
{
 	int startX = ChunkPosition.x - RenderDistance;
	int startY = ChunkPosition.y - RenderDistance;
	int EndX = ChunkPosition.x + RenderDistance;
	int EndY = ChunkPosition.y + RenderDistance;
	for (int x = startX; x <= EndX; x++)
	{
		for (int y = startY; y <= EndY; y++)
		{
			LoadNewChunk({ x, y });
		}
	}
}
void World::UnLoadPlayerChunks(Vector2<int> ChunkPosition, int RenderDistance)
{
	auto ChunkMapCopy = ChunkMap;
	for (auto& [value, chunk] : ChunkMapCopy)
	{
		Vector2<int> pos = ToVector(value);
		if (Math::Abs(pos.x - ChunkPosition.x) > RenderDistance || Math::Abs(pos.y - ChunkPosition.y) > RenderDistance)
		{
			delete chunk;
			ChunkMap.erase(value);
		}
	}
}
