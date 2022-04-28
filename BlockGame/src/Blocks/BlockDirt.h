#pragma once
#include "Block.h"
class BlockDirt : public Block
{
public:
	BLOCK_ID GetBlockId() override;
	BlockProperties GetBlockProperties() override;
};
