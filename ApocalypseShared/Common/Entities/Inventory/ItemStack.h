#pragma once
#include "pch.h"
enum class ItemStackType
{
	BlockItem, Item
};
typedef unsigned short ItemType;
class ItemStack
{
public:
	ItemStack();
	ItemStack(ItemStackType itemStackType,ItemType type, unsigned int count);
	inline unsigned int GetCount() { return count; };
	inline ItemStackType GetItemStackType() { return itemStackType; };
	inline ItemType GetItemType() { return itemType; };
private:
	ItemStackType itemStackType;
	ItemType itemType;
	unsigned int count;
};
