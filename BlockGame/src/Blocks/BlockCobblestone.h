#pragma once
#include "Block.h"
class BlockCobblestone : public Block
{
	BLOCK_ID GetBlockId() override;
	BlockProperties GetBlockProperties() override;
};
