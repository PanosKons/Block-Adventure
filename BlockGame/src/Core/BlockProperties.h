#pragma once
#include <Engine.h>
enum class TOOL
{
	Pickaxe, Shovel, Axe, None
};
struct BlockProperties //Each block is paired with some properties at global variables
{
	unsigned int hardness;
	TOOL tool;
	unsigned int miningLevel;
	std::array<unsigned char, 6> textureSides;
};
std::array<unsigned char, 6>& GetTexture(int id);
