#pragma once
enum class ITEM_ID
{
	DryGrassBlade, Stick, Pickaxe
};
enum class TYPE
{
	BLOCK, ITEM
};
class ItemStack
{
public:
	TYPE type;
	int id;
	unsigned int count;
	ItemStack(int id, unsigned int count, TYPE type);
	ItemStack();
};
