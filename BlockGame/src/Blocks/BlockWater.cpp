#include "Engine.h"
#include "BlockWater.h"
BLOCK_ID BlockWater::GetBlockId()
{
	return BLOCK_ID::Water;
}
BlockWater::BlockWater()
{
	//Transparent = true;
}
BlockProperties BlockWater::GetBlockProperties()
{
	return { 120,TOOL::Axe,0,{9,9,9,9,9,9} };
}