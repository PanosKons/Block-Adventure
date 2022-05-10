#pragma once
#include "Block.h"
class BlockAir : public Block
{
public:
	BlockAir();
	BLOCK_ID GetBlockId() override;
	BlockProperties GetBlockProperties() override;
};
