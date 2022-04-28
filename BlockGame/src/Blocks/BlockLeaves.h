#pragma once
#include "Block.h"
class BlockLeaves : public Block
{
public:
	BLOCK_ID GetBlockId() override;
	BlockLeaves();
	BlockProperties GetBlockProperties() override;
};
