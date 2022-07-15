#include <Engine.h>
#include "Networking.h"
#include "Math/Vector.h"
#include "BlockData.h"
#include "Math/Noise.h"
constexpr int RenderDistance = 1;
class Player
{
public:
	Vector3<int> CurrentChunkPosition = { -1123, -1432, -1754};
	bool ChangedChunk(Vector3<double> Position, Vector3<int>* ChunkPosi)
	{
		Vector3<int> ChunkPos = { (int)Position.x / ChunkSize ,(int)Position.y / ChunkSize,(int)Position.z / ChunkSize };
		if (ChunkPos != CurrentChunkPosition)
		{
			CurrentChunkPosition = ChunkPos;
			*ChunkPosi = ChunkPos;
			return true;
		}
		return false;
	}
};
static std::unordered_map<int, Player> PlayerMap;
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
void SendNewChunks(int Player_id, Vector3<int> ChunkPosition)
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
				std::array<char, defaultsize> buffer;
				*(int*)buffer.data() = (int)PACKET_ID::SendChunk;
				*(Vector3<int>*)(buffer.data() + sizeof(int)) = {x,y,z};
				send(*sockets[Player_id], buffer.data(), defaultsize, 0);
				std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks = CreateChunk({x,y,z});
				recv(*sockets[Player_id], (char*)buffer.data(), buffer.size(), 0);
				send(*sockets[Player_id], (char*)blocks->data(), ChunkSize * ChunkSize * ChunkSize * sizeof(BlockData), 0);
			}
		}
	}
	std::array<char, defaultsize> buffer;
	recv(*sockets[Player_id], (char*)buffer.data(), buffer.size(),0);

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