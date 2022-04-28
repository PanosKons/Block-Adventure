#include <Engine.h>
#include "BlockDirt.h"
BLOCK_ID BlockDirt::GetBlockId()
{
	return BLOCK_ID::Dirt;
}
BlockProperties BlockDirt::GetBlockProperties()
{
	return { 60,TOOL::Shovel,0,{2,2,2,2,2,2} };
}