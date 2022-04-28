#include <Engine.h>
#include "BlockGrass.h"
#include "../Core/GameManager.h"
#include "BlockAir.h"
BLOCK_ID BlockGrass::GetBlockId()
{
	return BLOCK_ID::Grass;
}
void BlockGrass::Update()
{
	Block::Update();
	Block* block = GameManager::Overworld->GetBlock({ Position.x, Position.y + 1, Position.z });
	if (block != nullptr)
	{
		if (block->GetBlockId() != BLOCK_ID::Air)
		{
			OnBreak(BLOCK_ID::Dirt);
		}
	}
}