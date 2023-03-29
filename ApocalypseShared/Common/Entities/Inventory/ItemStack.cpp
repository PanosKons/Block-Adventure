#include "pch.h"
#include "ItemStack.h"
ItemStack::ItemStack(int type, unsigned int count): ItemType(type), Count(count) {}
ItemStack::ItemStack() : ItemType(-1), Count(0) {}