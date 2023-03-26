#include "pch.h"
#include "WorldManager.h"
#include "Math/Noise.h"
#include "Math/EngineMath.h"
#include "Logger.h"

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
void WorldManager::ReplaceBlock(Block block,BlockType NewType)
{
	block.data->blockId = NewType;
	UpdateBlock(block);
	UpdateSurroundingBlocks(block);
	WorldManager::BaseWorld->GetChunkAbsolute(block.Position)->MeshChanged = true;
}
void WorldManager::UpdateBlock(Block block)
{
	if (block.GetBlockProperties().render == false) return;
	unsigned char oldSides = block.data->RenderedSides;
	std::array<Block,6> blocks =
	{
		WorldManager::BaseWorld->GetBlock({ block.Position.x + 1, block.Position.y, block.Position.z }),
		WorldManager::BaseWorld->GetBlock({ block.Position.x - 1, block.Position.y, block.Position.z }),
		WorldManager::BaseWorld->GetBlock({ block.Position.x, block.Position.y, block.Position.z + 1 }),
		WorldManager::BaseWorld->GetBlock({ block.Position.x, block.Position.y, block.Position.z - 1 }),
		WorldManager::BaseWorld->GetBlock({ block.Position.x, block.Position.y + 1, block.Position.z }),
		WorldManager::BaseWorld->GetBlock({ block.Position.x, block.Position.y - 1, block.Position.z })
	};
	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].IsValid())
			if (!blocks[i].GetBlockProperties().render || (blocks[i].GetBlockProperties().transparent && blocks[i].GetBlockId() != block.GetBlockId()))
			{
				block.data->RenderedSides = block.data->RenderedSides | Math::Powi(2,i);
			}
			else
			{
				block.data->RenderedSides = block.data->RenderedSides & (255 - Math::Powi(2, i));
			}
	}

	if (oldSides != block.data->RenderedSides)
	{
		Chunk* chunk = WorldManager::BaseWorld->GetChunkAbsolute(block.Position);
		if (chunk != nullptr) chunk->MeshChanged = true;
	}
}
void WorldManager::UpdateSurroundingBlocks(Block block)
{
	std::array<Block, 6> blocks =
	{
		WorldManager::BaseWorld->GetBlock({ block.Position.x + 1, block.Position.y, block.Position.z }),
		WorldManager::BaseWorld->GetBlock({ block.Position.x - 1, block.Position.y, block.Position.z }),
		WorldManager::BaseWorld->GetBlock({ block.Position.x, block.Position.y, block.Position.z + 1 }),
		WorldManager::BaseWorld->GetBlock({ block.Position.x, block.Position.y, block.Position.z - 1 }),
		WorldManager::BaseWorld->GetBlock({ block.Position.x, block.Position.y + 1, block.Position.z }),
		WorldManager::BaseWorld->GetBlock({ block.Position.x, block.Position.y - 1, block.Position.z })
	};
	for (unsigned int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].IsValid())
			if (blocks[i].GetBlockProperties().render)
			{
				UpdateBlock(blocks[i]);
			}
	}
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
					(*blocks)[x][y][z].blockId = Block::OreBlock; // WATER
				}
				else if (ylevel == level)
				{
					if (ylevel < 30)
					{
						(*blocks)[x][y][z].blockId = Block::DirtBlock;
					}
					else if (BiomeMap[x + z * ChunkSize] == 0)
					{
						(*blocks)[x][y][z].blockId = Block::StoneTopBlock;
					}
					else if (BiomeMap[x + z * ChunkSize] == 1)
					{
						(*blocks)[x][y][z].blockId = Block::DeadTopBlock;
					}
					else if (BiomeMap[x + z * ChunkSize] == 2)
					{
						(*blocks)[x][y][z].blockId = Block::DryTopBlock;
					}
					else
					{
						(*blocks)[x][y][z].blockId = Block::WetTopBlock;
					}
				}
				else if (ylevel + 1 == level)
				{
					(*blocks)[x][y][z].blockId = Block::DirtBlock;
				}
				else if (ylevel + 2 == level)
				{
					(*blocks)[x][y][z].blockId = Block::DirtBlock;
				}
				else if (ylevel < level)
				{
					if (rand() % 50 == 0)
						(*blocks)[x][y][z].blockId = Block::OreBlock;
					else
						(*blocks)[x][y][z].blockId = Block::UndergroundBlock;
				}
				else
				{
					(*blocks)[x][y][z].blockId = Block::FillerBlock;
				}
			}
		}
	}
	return blocks;
}
