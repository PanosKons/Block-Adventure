#pragma once
#include "../Blocks/Block.h"
class ItemStack
{
public:
	BLOCK_ID id;
	unsigned int count;
	ItemStack(BLOCK_ID id, unsigned int count);
	ItemStack();
};
