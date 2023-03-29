#include "pch.h"
#include "Common/Blocks/Block.h"
#include "World.h"
#include "Math/EngineMath.h"
#include "WorldManager.h"

World::World() : ChunkMap()
{
}
inline int positive_modulo(int f, int s) {
	return (s + (f % s)) % s;
}
Block World::GetBlock(Vector3<int> AbsolutePosition) const
{
	int x = Math::Floor(AbsolutePosition.x / (float)ChunkSize);
	int y = Math::Floor(AbsolutePosition.y / (float)ChunkSize);
	int z = Math::Floor(AbsolutePosition.z / (float)ChunkSize);
	auto it = ChunkMap.find(WorldManager::GetChunkKey({ x, y, z }));
	if (it != ChunkMap.end())
		return it->second->GetBlock({ positive_modulo(AbsolutePosition.x,ChunkSize), positive_modulo(AbsolutePosition.y,ChunkSize), positive_modulo(AbsolutePosition.z,ChunkSize) });
	return Block();
}
Chunk* World::GetChunkAbsolute(Vector3<int> AbsolutePosition) const
{
	int x = Math::Floor(AbsolutePosition.x / (float)ChunkSize);
	int y = Math::Floor(AbsolutePosition.y / (float)ChunkSize);
	int z = Math::Floor(AbsolutePosition.z / (float)ChunkSize);
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
}