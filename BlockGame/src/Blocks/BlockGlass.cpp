#include "Engine.h"
#include "BlockGlass.h"
BLOCK_ID BlockGlass::GetBlockId()
{
	return BLOCK_ID::Glass;
}
BlockGlass::BlockGlass()
{
	Transparent = true;
}