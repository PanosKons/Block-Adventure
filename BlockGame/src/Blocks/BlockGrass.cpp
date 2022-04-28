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
		if (!block->Transparent)
		{
			OnBreak(BLOCK_ID::Dirt);
		}
	}
}
BlockProperties BlockGrass::GetBlockProperties()
{
	return { 60,TOOL::Shovel,0,{1,1,1,1,2,0} };
}