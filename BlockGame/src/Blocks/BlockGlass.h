#pragma once
#include "Block.h"
class BlockGlass : public Block
{
public:
	BlockGlass();
	BLOCK_ID GetBlockId() override;
};
