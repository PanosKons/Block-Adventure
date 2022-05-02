#include <Engine.h>
#include "../Blocks.h"
#include "World.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Math/EngineMath.h"
#include "Timer.h"
#include "SavingData.h"
int64_t ToLong(short x, short y, short z)
{
	int64_t data = 0;
	short* p = (short*)&data;
	(*p) = x;
	p++;
	(*p) = y;
	p++;
	(*p) = z;
	return data;
}
Vector3<int> ToVector(int64_t value)
{
	Vector3<int> v;
	short* p = (short*)&value;
	v.x = *p;
	p++;
	v.y = *p;
	p++;
	v.z = *p;
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
Block* World::GetBlock(Vector3<int> AbsolutePosition) const
{
	int x = Math::Floor(AbsolutePosition.x / (float)ChunkSize);
	int y = Math::Floor(AbsolutePosition.y / (float)ChunkSize);
	int z = Math::Floor(AbsolutePosition.z / (float)ChunkSize);
	auto it = ChunkMap.find(ToLong(x, y, z));
	if (it != ChunkMap.end())
		return it->second->GetBlock({ (AbsolutePosition.x + BIG_NUMBER) % ChunkSize, (AbsolutePosition.y + BIG_NUMBER) % ChunkSize, (AbsolutePosition.z + BIG_NUMBER) % ChunkSize });
	return nullptr;
}
Chunk* World::GetChunk(Vector3<int> Position) const
{
	int x = Position.x / ChunkSize;
	int y = Position.y / ChunkSize;
	int z = Position.z / ChunkSize;
	auto it = ChunkMap.find(ToLong(x, y, z));
	if(it != ChunkMap.end())
		return it->second;
	return nullptr;
}

void World::LoadNewChunk(Vector3<int> ChunkPosition)
{
	if (ChunkMap.find(ToLong(ChunkPosition.x, ChunkPosition.y, ChunkPosition.z)) == ChunkMap.end())
	{
		Chunk* chunk = new Chunk({ ChunkPosition.x,ChunkPosition.y, ChunkPosition.z }, this);
		ChunkMap.emplace(ToLong(ChunkPosition.x, ChunkPosition.y, ChunkPosition.z), chunk);
		if (ChunkMap.find(ToLong(ChunkPosition.x + 1, ChunkPosition.y, ChunkPosition.z)) != ChunkMap.end())
			ChunkMap[ToLong(ChunkPosition.x + 1, ChunkPosition.y, ChunkPosition.z)]->ShouldUpdateBorders = true;
		if (ChunkMap.find(ToLong(ChunkPosition.x - 1, ChunkPosition.y, ChunkPosition.z)) != ChunkMap.end())
			ChunkMap[ToLong(ChunkPosition.x - 1, ChunkPosition.y, ChunkPosition.z)]->ShouldUpdateBorders = true;
		if (ChunkMap.find(ToLong(ChunkPosition.x, ChunkPosition.y + 1, ChunkPosition.z)) != ChunkMap.end())
			ChunkMap[ToLong(ChunkPosition.x, ChunkPosition.y + 1, ChunkPosition.z)]->ShouldUpdateBorders = true;
		if (ChunkMap.find(ToLong(ChunkPosition.x, ChunkPosition.y - 1, ChunkPosition.z)) != ChunkMap.end())
			ChunkMap[ToLong(ChunkPosition.x, ChunkPosition.y - 1, ChunkPosition.z)]->ShouldUpdateBorders = true;
		if (ChunkMap.find(ToLong(ChunkPosition.x, ChunkPosition.y, ChunkPosition.z + 1)) != ChunkMap.end())
			ChunkMap[ToLong(ChunkPosition.x, ChunkPosition.y, ChunkPosition.z + 1)]->ShouldUpdateBorders = true;
		if (ChunkMap.find(ToLong(ChunkPosition.x, ChunkPosition.y, ChunkPosition.z - 1)) != ChunkMap.end())
			ChunkMap[ToLong(ChunkPosition.x, ChunkPosition.y, ChunkPosition.z - 1)]->ShouldUpdateBorders = true;
	}
}
void World::SubmitChunkChanges()
{
	for (auto[id, chunk] : ChunkMap)
	{
		if (chunk->ShouldUpdate)
			chunk->UpdateAllBlocks();
		else if (chunk->ShouldUpdateBorders)
			chunk->UpdateBorderBlocks();
		chunk->ShouldUpdate = false;
		chunk->ShouldUpdateBorders = false;
	}
}
void World::LoadPlayerChunks(Vector3<int> ChunkPosition,int RenderDistance)
{
 	int startX = ChunkPosition.x - RenderDistance;
	int startY = ChunkPosition.y - RenderDistance;
	int startZ = ChunkPosition.z - RenderDistance;
	int EndX = ChunkPosition.x + RenderDistance;
	int EndY = ChunkPosition.y + RenderDistance;
	int EndZ = ChunkPosition.z + RenderDistance;
	for (int x = startX; x <= EndX; x++)
	{
		for (int y = startY; y <= EndY; y++)
		{
			for (int z = startZ; z <= EndZ; z++)
			{
				LoadNewChunk({ x, y, z });
			}
		}
	}
}
void World::UnLoadPlayerChunks(Vector3<int> ChunkPosition, int RenderDistance)
{
	auto ChunkMapCopy = ChunkMap;
	for (auto& [value, chunk] : ChunkMapCopy)
	{
		Vector3<int> pos = ToVector(value);
		if (Math::Abs(pos.x - ChunkPosition.x) > RenderDistance || Math::Abs(pos.y - ChunkPosition.y) > RenderDistance || Math::Abs(pos.z - ChunkPosition.z) > RenderDistance)
		{
			delete chunk;
			ChunkMap.erase(value);
		}
	}
}
