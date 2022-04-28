#include "Block.h"
#include "Renderer.h"
#include "World.h"
#include "GameManager.h"
#include <array>
Block::Block()
	:Transform(0), id(BLOCK_ID(0))
{
}
Block::~Block()
{
}
void Block::ChangeState(BLOCK_ID newState)
{
	id = newState;
	Update();
	std::array<Block*, 6> blocks =
	{
		GameManager::Overworld->GetBlock({ Transform.x + 1, Transform.y, Transform.z }),
		GameManager::Overworld->GetBlock({ Transform.x - 1, Transform.y, Transform.z }),
		GameManager::Overworld->GetBlock({ Transform.x, Transform.y, Transform.z + 1 }),
		GameManager::Overworld->GetBlock({ Transform.x, Transform.y, Transform.z - 1 }),
		GameManager::Overworld->GetBlock({ Transform.x, Transform.y + 1, Transform.z }),
		GameManager::Overworld->GetBlock({ Transform.x, Transform.y - 1, Transform.z })
	};
	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i] != nullptr)
			if (blocks[i]->id != BLOCK_ID::Air)
			{
				blocks[i]->Update();
			}
	}
	GameManager::Overworld->GetChunk(this)->Changed = true;
}
void Block::Update()
{
	unsigned char oldSides = RenderedSides;
	if (id != BLOCK_ID::Air) {
		Block* blocks[] =
		{
			GameManager::Overworld->GetBlock({ Transform.x + 1, Transform.y, Transform.z }),
			GameManager::Overworld->GetBlock({ Transform.x - 1, Transform.y, Transform.z }),
			GameManager::Overworld->GetBlock({ Transform.x, Transform.y, Transform.z + 1 }),
			GameManager::Overworld->GetBlock({ Transform.x, Transform.y, Transform.z - 1 }),
			GameManager::Overworld->GetBlock({ Transform.x, Transform.y + 1, Transform.z }),
			GameManager::Overworld->GetBlock({ Transform.x, Transform.y - 1, Transform.z })
		};
		if (blocks[0] != nullptr)
			if (blocks[0]->id == BLOCK_ID::Air)
			{
				RenderedSides = RenderedSides | 2;
			}
			else
			{
				RenderedSides = RenderedSides & (63 - 2);
			}
		if (blocks[1] != nullptr)
			if (blocks[1]->id == BLOCK_ID::Air)
			{
				RenderedSides = RenderedSides | 8;
			}
			else
			{
				RenderedSides = RenderedSides & (63 - 8);
			}
		if (blocks[2] != nullptr)
			if (blocks[2]->id == BLOCK_ID::Air)
			{
				RenderedSides = RenderedSides | 4;
			}
			else
			{
				RenderedSides = RenderedSides & (63 - 4);
			}
		if (blocks[3] != nullptr)
			if (blocks[3]->id == BLOCK_ID::Air)
			{
				RenderedSides = RenderedSides | 1;
			}
			else
			{
				RenderedSides = RenderedSides & (63 - 1);
			}
		if (blocks[4] != nullptr)
			if (blocks[4]->id == BLOCK_ID::Air)
			{
				RenderedSides = RenderedSides | 32;
			}
			else
			{
				RenderedSides = RenderedSides & (63 - 32);
			}
		if (blocks[5] != nullptr)
			if (blocks[5]->id == BLOCK_ID::Air)
			{
				RenderedSides = RenderedSides | 16;
			}
			else
			{
				RenderedSides = RenderedSides & (63 - 16);
			}
	}
	else
	{
		RenderedSides = 0;
	}
	if (oldSides != RenderedSides)
		GameManager::Overworld->GetChunk(this)->Changed = true;
}