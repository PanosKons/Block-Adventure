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
		delete element.second;
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
Block World::MakeBlock(BlockData* data,Vector3<int> Position) //Has nothing to do with world
{
	return Block(Position,data);
}
World::World(int seed)
{
	GameManager::Overworld = this;
}
Block World::GetBlock(Vector3<int> AbsolutePosition) const
{
	int x = Math::Floor(AbsolutePosition.x / (float)ChunkSize);
	int y = Math::Floor(AbsolutePosition.y / (float)ChunkSize);
	int z = Math::Floor(AbsolutePosition.z / (float)ChunkSize);
	auto it = ChunkMap.find(ToLong(x, y, z));
	if (it != ChunkMap.end())
		return it->second->GetBlock({ (AbsolutePosition.x + BIG_NUMBER) % ChunkSize, (AbsolutePosition.y + BIG_NUMBER) % ChunkSize, (AbsolutePosition.z + BIG_NUMBER) % ChunkSize });
	return Block();
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
//Chunk* LoadChunkAsync(Vector3<int> ChunkPosition, World* world)
//{
//	return new Chunk(ChunkPosition, world);
//}
//void World::LoadPlayerChunks(Vector3<int> ChunkPosition,int RenderDistance)
//{
// 	int startX = ChunkPosition.x - RenderDistance;
//	int startY = ChunkPosition.y - RenderDistance;
//	int startZ = ChunkPosition.z - RenderDistance;
//	int EndX = ChunkPosition.x + RenderDistance;
//	int EndY = ChunkPosition.y + RenderDistance;
//	int EndZ = ChunkPosition.z + RenderDistance;
//	std::vector<std::future<Chunk*>> futures;
//	for (int x = startX; x <= EndX; x++)
//	{
//		for (int y = startY; y <= EndY; y++)
//		{
//			for (int z = startZ; z <= EndZ; z++)
//			{
//				if (ChunkMap.find(ToLong(x, y, z)) == ChunkMap.end())
//				{
//					Vector3<int> v = { x,y,z };
//					futures.push_back(std::async(std::launch::async, LoadChunkAsync, v, this));
//				}
//			}
//		}
//	}
//	for (int i = 0; i < futures.size(); i++)
//	{
//		Chunk* chunk = futures[i].get();
//		ChunkMap.emplace(ToLong(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z), chunk);
//		if (ChunkMap.find(ToLong(chunk->GetPosition().x + 1, chunk->GetPosition().y, chunk->GetPosition().z)) != ChunkMap.end())
//			ChunkMap[ToLong(chunk->GetPosition().x + 1, chunk->GetPosition().y, chunk->GetPosition().z)]->ShouldUpdateBorders = true;
//		if (ChunkMap.find(ToLong(chunk->GetPosition().x - 1, chunk->GetPosition().y, chunk->GetPosition().z)) != ChunkMap.end())
//			ChunkMap[ToLong(chunk->GetPosition().x - 1, chunk->GetPosition().y, chunk->GetPosition().z)]->ShouldUpdateBorders = true;
//		if (ChunkMap.find(ToLong(chunk->GetPosition().x, chunk->GetPosition().y + 1, chunk->GetPosition().z)) != ChunkMap.end())
//			ChunkMap[ToLong(chunk->GetPosition().x, chunk->GetPosition().y + 1, chunk->GetPosition().z)]->ShouldUpdateBorders = true;
//		if (ChunkMap.find(ToLong(chunk->GetPosition().x, chunk->GetPosition().y - 1, chunk->GetPosition().z)) != ChunkMap.end())
//			ChunkMap[ToLong(chunk->GetPosition().x, chunk->GetPosition().y - 1, chunk->GetPosition().z)]->ShouldUpdateBorders = true;
//		if (ChunkMap.find(ToLong(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z + 1)) != ChunkMap.end())
//			ChunkMap[ToLong(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z + 1)]->ShouldUpdateBorders = true;
//		if (ChunkMap.find(ToLong(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z - 1)) != ChunkMap.end())
//			ChunkMap[ToLong(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z - 1)]->ShouldUpdateBorders = true;
//	}
//}
//void World::UnLoadPlayerChunks(Vector3<int> ChunkPosition, int RenderDistance)
//{
//	auto ChunkMapCopy = ChunkMap;
//	for (auto& [value, chunk] : ChunkMapCopy)
//	{
//		Vector3<int> pos = ToVector(value);
//		if (Math::Abs(pos.x - ChunkPosition.x) > RenderDistance || Math::Abs(pos.y - ChunkPosition.y) > RenderDistance || Math::Abs(pos.z - ChunkPosition.z) > RenderDistance)
//		{
//			ChunkMap.erase(value);
//			delete chunk;
//		}
//	}
//}
void World::MakeNewChunk(Vector3<int> ChunkPosition, std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks)
{
	Chunk* chunk = new Chunk(ChunkPosition,blocks, this);
	ChunkMap.emplace(ToLong(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z), chunk);
	if (ChunkMap.find(ToLong(chunk->GetPosition().x + 1, chunk->GetPosition().y, chunk->GetPosition().z)) != ChunkMap.end())
		ChunkMap[ToLong(chunk->GetPosition().x + 1, chunk->GetPosition().y, chunk->GetPosition().z)]->ShouldUpdateBorders = true;
	if (ChunkMap.find(ToLong(chunk->GetPosition().x - 1, chunk->GetPosition().y, chunk->GetPosition().z)) != ChunkMap.end())
		ChunkMap[ToLong(chunk->GetPosition().x - 1, chunk->GetPosition().y, chunk->GetPosition().z)]->ShouldUpdateBorders = true;
	if (ChunkMap.find(ToLong(chunk->GetPosition().x, chunk->GetPosition().y + 1, chunk->GetPosition().z)) != ChunkMap.end())
		ChunkMap[ToLong(chunk->GetPosition().x, chunk->GetPosition().y + 1, chunk->GetPosition().z)]->ShouldUpdateBorders = true;
	if (ChunkMap.find(ToLong(chunk->GetPosition().x, chunk->GetPosition().y - 1, chunk->GetPosition().z)) != ChunkMap.end())
		ChunkMap[ToLong(chunk->GetPosition().x, chunk->GetPosition().y - 1, chunk->GetPosition().z)]->ShouldUpdateBorders = true;
	if (ChunkMap.find(ToLong(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z + 1)) != ChunkMap.end())
		ChunkMap[ToLong(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z + 1)]->ShouldUpdateBorders = true;
	if (ChunkMap.find(ToLong(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z - 1)) != ChunkMap.end())
		ChunkMap[ToLong(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z - 1)]->ShouldUpdateBorders = true;
}
void World::DestroyChunk(Vector3<int> ChunkPosition)
{
	Chunk* chunk = ChunkMap[ToLong(ChunkPosition.x, ChunkPosition.y, ChunkPosition.z)];
	ChunkMap.erase(ToLong(ChunkPosition.x, ChunkPosition.y, ChunkPosition.z));
	delete chunk;
}