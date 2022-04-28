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