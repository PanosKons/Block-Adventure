#include <Engine.h>
#include "BlockDryGrass.h"
#include "../Core/GameManager.h"
BLOCK_ID BlockDryGrass::GetBlockId()
{
	return BLOCK_ID::DryGrass;
}
void BlockDryGrass::Update()
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
BlockProperties BlockDryGrass::GetBlockProperties()
{
	return { 60,TOOL::Shovel,0,{11,11,11,11,2,10} };
}