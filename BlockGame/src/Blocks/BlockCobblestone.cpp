#include <Engine.h>
#include "BlockCobblestone.h"
BLOCK_ID BlockCobblestone::GetBlockId()
{
	return BLOCK_ID::Cobblestone;
}
BlockProperties BlockCobblestone::GetBlockProperties()
{
	return { 180,TOOL::Pickaxe,0,{3,3,3,3,3,3} };
}