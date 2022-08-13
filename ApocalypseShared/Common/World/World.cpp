#include "pch.h"
#include "Common/Blocks/Block.h"
#include "World.h"
#include "Math/EngineMath.h"
#include "WorldManager.h"
#include "Math/Noise.h"

World::World(){}

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
Chunk* World::GetChunk(Vector3<int> Position) const
{
	int x = Position.x / ChunkSize;
	int y = Position.y / ChunkSize;
	int z = Position.z / ChunkSize;
	auto it = ChunkMap.find(WorldManager::GetChunkKey({ x, y, z }));
	if (it != ChunkMap.end())
		return it->second;
	return nullptr;
}

void World::CreateChunk(Vector3<int> ChunkPosition, BlockArray* blocks)
{
	Chunk* chunk = new Chunk(ChunkPosition, this, blocks);
	//ChunkMap.emplace(WorldManager::GetChunkKey(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z), chunk);
	//if (ChunkMap.find(WorldManager::GetChunkKey(chunk->GetPosition().x + 1, chunk->GetPosition().y, chunk->GetPosition().z)) != ChunkMap.end())
	//	ChunkMap[WorldManager::GetChunkKey(chunk->GetPosition().x + 1, chunk->GetPosition().y, chunk->GetPosition().z)]->ShouldUpdateBorders = true;
	//if (ChunkMap.find(WorldManager::GetChunkKey(chunk->GetPosition().x - 1, chunk->GetPosition().y, chunk->GetPosition().z)) != ChunkMap.end())
	//	ChunkMap[WorldManager::GetChunkKey(chunk->GetPosition().x - 1, chunk->GetPosition().y, chunk->GetPosition().z)]->ShouldUpdateBorders = true;
	//if (ChunkMap.find(WorldManager::GetChunkKey(chunk->GetPosition().x, chunk->GetPosition().y + 1, chunk->GetPosition().z)) != ChunkMap.end())
	//	ChunkMap[WorldManager::GetChunkKey(chunk->GetPosition().x, chunk->GetPosition().y + 1, chunk->GetPosition().z)]->ShouldUpdateBorders = true;
	//if (ChunkMap.find(WorldManager::GetChunkKey(chunk->GetPosition().x, chunk->GetPosition().y - 1, chunk->GetPosition().z)) != ChunkMap.end())
	//	ChunkMap[WorldManager::GetChunkKey(chunk->GetPosition().x, chunk->GetPosition().y - 1, chunk->GetPosition().z)]->ShouldUpdateBorders = true;
	//if (ChunkMap.find(WorldManager::GetChunkKey(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z + 1)) != ChunkMap.end())
	//	ChunkMap[WorldManager::GetChunkKey(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z + 1)]->ShouldUpdateBorders = true;
	//if (ChunkMap.find(WorldManager::GetChunkKey(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z - 1)) != ChunkMap.end())
	//	ChunkMap[WorldManager::GetChunkKey(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z - 1)]->ShouldUpdateBorders = true;
}
void World::DestroyChunk(Vector3<int> ChunkPosition)
{
	Chunk* chunk = ChunkMap[WorldManager::GetChunkKey(ChunkPosition)];
	ChunkMap.erase(WorldManager::GetChunkKey(ChunkPosition));
	delete chunk;
	//inform the others?
}

BlockArray* CreateChunk(Vector3<int> ChunkPosition)
{
	std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks = new std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>();
	srand(1);
	std::array<int, ChunkSize* ChunkSize> HeightMap;
	std::array<int, ChunkSize* ChunkSize> BiomeMap;
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int z = 0; z < ChunkSize; z++)
		{
			HeightMap[x + z * ChunkSize] = Noise::GetYLevel(x + ChunkPosition.x * ChunkSize, z + ChunkPosition.z * ChunkSize);
			BiomeMap[x + z * ChunkSize] = Noise::GetBiomeTemperature(x + ChunkPosition.x * ChunkSize, z + ChunkPosition.z * ChunkSize);
		}
	}
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				int ylevel = y + ChunkSize * ChunkPosition.y;
				int level = HeightMap[x + z * ChunkSize];
				if (ylevel > level && ylevel <= 30)
				{
					(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Water;
				}
				else if (ylevel == level)
				{
					if (ylevel < 30)
					{
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Dirt;
					}
					else if (BiomeMap[x + z * ChunkSize] == 0)
					{
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Cobblestone;
					}
					else if (BiomeMap[x + z * ChunkSize] == 1)
					{
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Dirt;
					}
					else if (BiomeMap[x + z * ChunkSize] == 2)
					{
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::DryGrass;
					}
					else
					{
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Grass;
					}
				}
				else if (ylevel + 1 == level)
				{
					(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Dirt;
				}
				else if (ylevel + 2 == level)
				{
					(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Dirt;;
				}
				else if (ylevel < level)
				{
					if (rand() % 50 == 0)
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Iron;
					else
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Cobblestone;
				}
				else
				{
					(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Air;
				}
			}
		}
	}
	return blocks;
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