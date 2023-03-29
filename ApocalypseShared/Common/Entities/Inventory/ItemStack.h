#pragma once
#include "pch.h"
class ItemStack
{
public:
	ItemStack();
	ItemStack(int type, unsigned int count);
	inline unsigned int GetCount() { return Count; };
	inline int GetItemType() { return ItemType; };
	inline bool IsValid() { return ItemType >= 0; };
	int ItemType;
	int Count;
};
