#include <Engine.h>
#include "ItemStack.h"
ItemStack::ItemStack(BLOCK_ID id, unsigned int count)
	:id(id), count(count) {}
ItemStack::ItemStack()
	: id((BLOCK_ID)0), count(0) {}