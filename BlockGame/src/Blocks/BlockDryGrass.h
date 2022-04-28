#pragma once
#include "Block.h"
class BlockDryGrass : public Block
{
	BLOCK_ID GetBlockId() override;
	void Update() override;
	BlockProperties GetBlockProperties() override;
};
