#pragma once
#include "Block.h"
class BlockLog : public Block
{
	BLOCK_ID GetBlockId() override;
	BlockProperties GetBlockProperties() override;
};
