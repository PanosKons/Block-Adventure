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
BlockProperties BlockGlass::GetBlockProperties()
{
	return { 60,TOOL::None,0,{7,7,7,7,7,7} };
}