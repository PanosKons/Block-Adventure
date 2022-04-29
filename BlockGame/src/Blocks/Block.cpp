#include <Engine.h>
#include "Block.h"
#include "../Core/GameManager.h"
#include <array>
#include <iostream>
void Block::StateChanged()
{
	std::array<Block*, 6> blocks =
	{
		GameManager::Overworld->GetBlock({ Position.x + 1, Position.y, Position.z }),
		GameManager::Overworld->GetBlock({ Position.x - 1, Position.y, Position.z }),
		GameManager::Overworld->GetBlock({ Position.x, Position.y, Position.z + 1 }),
		GameManager::Overworld->GetBlock({ Position.x, Position.y, Position.z - 1 }),
		GameManager::Overworld->GetBlock({ Position.x, Position.y + 1, Position.z }),
		GameManager::Overworld->GetBlock({ Position.x, Position.y - 1, Position.z })
	};
	for (unsigned int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i] != nullptr)
			if (blocks[i]->GetBlockId() != BLOCK_ID::Air)
			{
				blocks[i]->Update();
			}
	}
}
void Block::OnBreak(BLOCK_ID id)
{
	auto a = GameManager::Overworld->GetChunk(this)->blocks;
	auto b = &(*a)[(Position.x + BIG_NUMBER) % ChunkSize][Position.y][(Position.z + BIG_NUMBER) % ChunkSize];
	(*b) = GameManager::Overworld->MakeBlock(id);
	(*b)->Position = Position;
	(*b)->Update();
	StateChanged();
	GameManager::Overworld->GetChunk(this)->Changed = true;
	delete this;
}
Block::Block()
	:RenderedSides(64), Transparent(false)
{}
void Block::Update()
{
	unsigned char oldSides = RenderedSides;
	Block* blocks[] =
	{
		GameManager::Overworld->GetBlock({ Position.x + 1, Position.y, Position.z }),
		GameManager::Overworld->GetBlock({ Position.x - 1, Position.y, Position.z }),
		GameManager::Overworld->GetBlock({ Position.x, Position.y, Position.z + 1 }),
		GameManager::Overworld->GetBlock({ Position.x, Position.y, Position.z - 1 }),
		GameManager::Overworld->GetBlock({ Position.x, Position.y + 1, Position.z }),
		GameManager::Overworld->GetBlock({ Position.x, Position.y - 1, Position.z })
	};
	if (blocks[0] != nullptr)
		if (blocks[0]->Transparent && blocks[0]->GetBlockId() != GetBlockId())
		{
			RenderedSides = RenderedSides | 2;
		}
		else
		{
			RenderedSides = RenderedSides & (255 - 2);
		}
	if (blocks[1] != nullptr)
		if (blocks[1]->Transparent && blocks[1]->GetBlockId() != GetBlockId())
		{
			RenderedSides = RenderedSides | 8;
		}
		else
		{
			RenderedSides = RenderedSides & (255 - 8);
		}
	if (blocks[2] != nullptr)
		if (blocks[2]->Transparent && blocks[2]->GetBlockId() != GetBlockId())
		{
			RenderedSides = RenderedSides | 4;
		}
		else
		{
			RenderedSides = RenderedSides & (255 - 4);
		}
	if (blocks[3] != nullptr)
		if (blocks[3]->Transparent && blocks[3]->GetBlockId() != GetBlockId())
		{
			RenderedSides = RenderedSides | 1;
		}
		else
		{
			RenderedSides = RenderedSides & (255 - 1);
		}
	if (blocks[4] != nullptr)
		if (blocks[4]->Transparent && blocks[4]->GetBlockId() != GetBlockId())
		{
			RenderedSides = RenderedSides | 32;
		}
		else
		{
			RenderedSides = RenderedSides & (255 - 32);
		}
	if (blocks[5] != nullptr)
		if (blocks[5]->Transparent && blocks[5]->GetBlockId() != GetBlockId())
		{
			RenderedSides = RenderedSides | 16;
		}
		else
		{
			RenderedSides = RenderedSides & (255 - 16);
		}

	if (oldSides != RenderedSides)
		GameManager::Overworld->GetChunk(this)->Changed = true;
}