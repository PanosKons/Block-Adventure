#include "pch.h"
#include "Blocks/Block.h"
#include "Chunk.h"
#include "World.h"
#include "Math/Noise.h"
#include "WorldManager.h"
Chunk::Chunk(Vector3<int> Position, World* world,BlockArray* blocks)
	:Position(Position), world(world), blocks(blocks) {}
Chunk::~Chunk()
{
	delete blocks;
}
void Chunk::Refresh()
{
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				GetBlock({ x,y,z }).Update();
			}
		}
	}
	MeshChanged = false;
}
void Chunk::RefreshBorders()
{
	for (int x = 0; x < ChunkSize; x += ChunkSize - 1)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				GetBlock({ x,y,z }).Update();
			}
		}
	}
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 1; z < ChunkSize; z += ChunkSize - 2)
			{
				GetBlock({ x,y,z }).Update();
			}
		}
	}
	for (int x = 1; x < ChunkSize - 1; x++)
	{
		for (int y = 0; y < ChunkSize; y += ChunkSize - 1)
		{
			for (int z = 1; z < ChunkSize - 1; z++)
			{
				GetBlock({ x,y,z }).Update();
			}
		}
	}
}

Block Chunk::GetBlock(Vector3<int> RelativePosition) const
{
	return WorldManager::GetBlock(&(*blocks)[RelativePosition.x][RelativePosition.y][RelativePosition.z], { RelativePosition.x + Position.x * ChunkSize,RelativePosition.y + Position.y * ChunkSize, RelativePosition.z + Position.z * ChunkSize });
}