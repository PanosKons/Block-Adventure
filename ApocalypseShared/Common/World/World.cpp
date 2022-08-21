#include "pch.h"
#include "Common/Blocks/Block.h"
#include "World.h"
#include "Math/EngineMath.h"
#include "WorldManager.h"

World::World() : ChunkMap()
{
}

Block World::GetBlock(Vector3<int> AbsolutePosition) const
{
	int x = Math::Floor(AbsolutePosition.x / (float)ChunkSize);
	int y = Math::Floor(AbsolutePosition.y / (float)ChunkSize);
	int z = Math::Floor(AbsolutePosition.z / (float)ChunkSize);
	auto it = ChunkMap.find(WorldManager::GetChunkKey({ x, y, z }));
	if (it != ChunkMap.end())
		return it->second->GetBlock({ (AbsolutePosition.x + BIG_NUMBER) % ChunkSize, (AbsolutePosition.y + BIG_NUMBER) % ChunkSize, (AbsolutePosition.z + BIG_NUMBER) % ChunkSize });
	return Block();
}
Chunk* World::GetChunkAbsolute(Vector3<int> AbsolutePosition) const
{
	int x = AbsolutePosition.x / ChunkSize;
	int y = AbsolutePosition.y / ChunkSize;
	int z = AbsolutePosition.z / ChunkSize;
	auto it = ChunkMap.find(WorldManager::GetChunkKey({ x, y, z }));
	if (it != ChunkMap.end())
		return it->second;
	return nullptr;
}
Chunk* World::GetChunkDirect(Vector3<int> ChunkPosition) const
{
	auto it = ChunkMap.find(WorldManager::GetChunkKey(ChunkPosition));
	if (it != ChunkMap.end())
		return it->second;
	return nullptr;
}

void World::CreateChunk(Vector3<int> ChunkPosition, BlockArray* blocks)
{
	Chunk* chunk = new Chunk(ChunkPosition, this, blocks);
	ChunkMap.emplace(WorldManager::GetChunkKey(chunk->GetPosition()),chunk);
}
void World::DestroyChunk(Vector3<int> ChunkPosition)
{
	Chunk* chunk = ChunkMap[WorldManager::GetChunkKey(ChunkPosition)];
	ChunkMap.erase(WorldManager::GetChunkKey(ChunkPosition));
	delete chunk;
	//inform the others?
}

//void World::SubmitChunkChanges()
//{
//	for (auto[id, chunk] : ChunkMap)
//	{
//		if (chunk->ShouldUpdate)
//			chunk->UpdateAllBlocks();
//		else if (chunk->ShouldUpdateBorders)
//			chunk->UpdateBorderBlocks();
//		chunk->ShouldUpdate = false;
//		chunk->ShouldUpdateBorders = false;
//	}
//}