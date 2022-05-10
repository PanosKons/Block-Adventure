#include <Engine.h>
#include "BlockAir.h"
#include "../Core/GameManager.h"
BLOCK_ID BlockAir::GetBlockId()
{
	return BLOCK_ID::Air;
}
BlockAir::BlockAir()
{
	RenderedSides = 0;
	Transparent = true;
}
BlockProperties BlockAir::GetBlockProperties()
{
	return { 0,TOOL::None,0, {0,0,0,0,0,0} };
}