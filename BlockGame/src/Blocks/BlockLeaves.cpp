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