#pragma once
#include "Block.h"
#include "SpecialProperties/IClickable.h"
class BlockDryGrass : public Block, public IClickable
{
	BLOCK_ID GetBlockId() override;
	void Update() override;
	void OnClick() override;
	BlockProperties GetBlockProperties() override;
};
