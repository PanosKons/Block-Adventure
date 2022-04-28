#include <Engine.h>
#include "GlobalVariables.h"
int ScreenWidth;
int ScreenHeight;
GLFWwindow* ApplicationWindow;
int FPS;
bool Playing = true;
std::unordered_map<BLOCK_ID, BlockProperties> blockProperties =
{
	{BLOCK_ID::Grass,{{60},{TOOL::Shovel},{0}, {1,1,1,1,2,0}}},
	{BLOCK_ID::Cobblestone,{{180},{TOOL::Pickaxe},{0},{3,3,3,3,3,3}}},
	{BLOCK_ID::Log,{{120},{TOOL::Axe},{0},{4,4,4,4,5,5}}},
	{BLOCK_ID::Iron,{{300},{TOOL::Pickaxe},{0},{6,6,6,6,6,6}}},
	{BLOCK_ID::Dirt,{{60},{TOOL::Shovel},{0},{2,2,2,2,2,2}}},
	{BLOCK_ID::Glass,{{30},{TOOL::Axe},{0},{7,7,7,7,7,7}}},
	{BLOCK_ID::Leaves,{{5},{TOOL::Axe},{0},{8,8,8,8,8,8}}},
	{BLOCK_ID::Water,{{5},{TOOL::Shovel},{0},{9,9,9,9,9,9}}},
};