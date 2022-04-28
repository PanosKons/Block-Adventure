#include "Engine.h"
#include "BlockLeaves.h"
BlockLeaves::BlockLeaves()
{
	Transparent = true;
}
BLOCK_ID BlockLeaves::GetBlockId()
{
	return BLOCK_ID::Leaves;
}
BlockProperties BlockLeaves::GetBlockProperties()
{
	return { 5,TOOL::Axe,0,{8,8,8,8,8,8} };
}