#include <Engine.h>
#include "BlockLog.h"
#include "../Core/GameManager.h"
#include "BlockAir.h"
BLOCK_ID BlockLog::GetBlockId()
{
	return BLOCK_ID::Log;
}
BlockProperties BlockLog::GetBlockProperties()
{
	return { 120,TOOL::Axe,0,{4,4,4,4,5,5} };
}