#include "pch.h"
#include "ItemStack.h"
ItemStack::ItemStack(ItemStackType itemStackType,ItemType type, unsigned int count):itemStackType(itemStackType),itemType(type), count(count) {}
ItemStack::ItemStack() : itemType(0), count(0), itemStackType(ItemStackType::Item) {}