#include <Engine.h>
#include "Networking.h"
#include "Math/Vector.h"
#include "BlockData.h"
#include "Math/Noise.h"
std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* CreateChunk(Vector3<int> ChunkPosition)
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
int main()
{
	std::thread network(Networking::Start);
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20)); //50fps
		Networking::Update();
	}
	Networking::Shutdown();
	return 0;
}