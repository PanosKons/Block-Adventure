#pragma once
#include "Block.h"
class BlockGrass :public Block
{
	BLOCK_ID GetBlockId() override;
	void Update() override;
};
