#pragma once
#include "Block.h"
class BlockWater :public Block
{
public:
	BlockWater();
	BLOCK_ID GetBlockId() override;
	BlockProperties GetBlockProperties() override;
};
