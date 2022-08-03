#include "pch.h"
#include "ItemStack.h"
ItemStack::ItemStack(int id, unsigned int count, TYPE type)
	:id(id), count(count),type(type) {}
ItemStack::ItemStack()
	: id(0), count(0),type(TYPE::ITEM) {}