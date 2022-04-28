#include <Engine.h>
#include "BlockIron.h"
BLOCK_ID BlockIron::GetBlockId()
{
	return BLOCK_ID::Iron;
}
BlockProperties BlockIron::GetBlockProperties()
{
	return { 300,TOOL::Pickaxe,0,{6,6,6,6,6,6} };
}