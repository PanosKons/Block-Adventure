#pragma once
#include <array>
enum class TOOL
{
	Pickaxe, Shovel, Axe
};
struct BlockProperties
{
	unsigned int hardness;
	TOOL tool;
	unsigned int miningLevel;
	std::array<unsigned char, 6> textureSides;
};
