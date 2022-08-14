#include "pch.h"
#include "WorldManager.h"
#include "Math/Noise.h"

Block WorldManager::GetBlock(BlockData* data, Vector3<int> WorldPosition)
{
    return Block(WorldPosition,data);
}
int64_t WorldManager::GetChunkKey(Vector3<int> position)
{
	int64_t data = 0;
	short* p = (short*)&data;
	(*p) = (short)position.x;
	p++;
	(*p) = (short)position.y;
	p++;
	(*p) = (short)position.z;
	return data;
}
Vector3<int> WorldManager::GetVectorFromKey(int64_t value)
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
BlockArray* WorldManager::GenerateChunk(Vector3<int> ChunkPosition)
{
	BlockArray* blocks = new BlockArray();
	//srand(1);
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
