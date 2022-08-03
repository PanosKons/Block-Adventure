#include "pch.h"
#include "Blocks/Block.h"
#include "Chunk.h"
#include "World.h"
#include "Math/Noise.h"
#include "WorldManager.h"
/*
void Chunk::SpawnStructure(Vector3<int> RelativePosition, std::string&& name)
{
	static Structure* structure = SavingData::LoadStructure(name.c_str());
	for (int x = 0; x < StructureSize; x++)
	{
		for (int y = 0; y < StructureSize; y++)
		{
			for (int z = 0; z < StructureSize; z++)
			{
				if (structure->data[x + y * StructureSize + z * StructureSize * StructureSize] == BLOCK_ID::Air) continue;
				if (RelativePosition.x + x < ChunkSize && RelativePosition.y + y < ChunkSize && RelativePosition.z + z < ChunkSize)
				{
					(*blocks)[RelativePosition.x + x][RelativePosition.y + y][RelativePosition.z + z].blockId = (unsigned short)structure->data[x + y * StructureSize + z * StructureSize * StructureSize];
				}
			}
		}
	}
}
*/

Chunk::Chunk(Vector3<int> Position, World* world,BlockArray* blocks)
	:Position(Position), world(world), blocks(blocks) {}
Chunk::~Chunk()
{
	delete blocks;
}
void UpdateAllBlocksAsync(std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks,Chunk* chunk)
{
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				chunk->GetBlock({ x,y,z }).Update();
			}
		}
	}
}
void UpdateBorderBlocksAsync(std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks, Chunk* chunk)
{
	for (int x = 0; x < ChunkSize; x += ChunkSize - 1)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				chunk->GetBlock({ x,y,z }).Update();
			}
		}
	}
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 1; z < ChunkSize; z += ChunkSize - 2)
			{
				chunk->GetBlock({ x,y,z }).Update();
			}
		}
	}
	for (int x = 1; x < ChunkSize - 1; x++)
	{
		for (int y = 0; y < ChunkSize; y += ChunkSize - 1)
		{
			for (int z = 1; z < ChunkSize - 1; z++)
			{
				chunk->GetBlock({ x,y,z }).Update();
			}
		}
	}
}
void Chunk::UpdateAllBlocks()
{
	std::thread worker(UpdateAllBlocksAsync,blocks,this);
	worker.detach();
}
void Chunk::UpdateBorderBlocks()
{
	std::thread worker(UpdateBorderBlocksAsync, blocks,this);
	worker.detach();
}

Block Chunk::GetBlock(Vector3<int> RelativePosition) const
{
	return WorldManager::GetBlock(&(*blocks)[RelativePosition.x][RelativePosition.y][RelativePosition.z], { RelativePosition.x + Position.x * ChunkSize,RelativePosition.y + Position.y * ChunkSize, RelativePosition.z + Position.z * ChunkSize });
}