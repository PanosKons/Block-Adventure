#include "BlockProperties.h"
#include "../Blocks/Block.h"
static std::unordered_map<int, std::array<unsigned char, 6>> Textures =
{
	{(int)BLOCK_ID::Air,{0,0,0,0,0,0}},
	{(int)BLOCK_ID::Cobblestone,{3,3,3,3,3,3}},
	{(int)BLOCK_ID::Dirt,{2,2,2,2,2,2}},
	{(int)BLOCK_ID::Glass,{7,7,7,7,7,7}},
	{(int)BLOCK_ID::Grass,{1,1,1,1,2,0}},
	{(int)BLOCK_ID::Iron,{6,6,6,6,6,6}},
	{(int)BLOCK_ID::Leaves,{8,8,8,8,8,8}},
	{(int)BLOCK_ID::Log,{4,4,4,4,5,5}},
	{(int)BLOCK_ID::Water,{9,9,9,9,9,9}},
	{(int)BLOCK_ID::DryGrass,{11,11,11,11,2,10}},

};
std::array<unsigned char, 6>& GetTexture(int id)
{
	return Textures[id];
}