#pragma once
#include <array>
enum class TOOL
{
	Pickaxe, Shovel, Axe
};
struct BlockProperties //Each block is paired with some properties at global variables
{
	unsigned int hardness;
	TOOL tool;
	unsigned int miningLevel;
	std::array<unsigned char, 6> textureSides;
};
