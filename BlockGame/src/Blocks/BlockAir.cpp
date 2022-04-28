#include <Engine.h>
#include "BlockAir.h"
#include "../Core/GameManager.h"
BLOCK_ID BlockAir::GetBlockId()
{
	return BLOCK_ID::Air;
}
void BlockAir::Update() {}
BlockAir::BlockAir()
{
	RenderedSides = 0;
	Transparent = true;
}