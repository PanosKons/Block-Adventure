#pragma once
#include "Common/Math/Vector.h"
#include "Common/World/World.h"
#include "Common/Blocks/BlockData.h"
#include "Common/Blocks/Block.h"
class WorldManager
{
public:
	static Block GetBlock(BlockData* data, Vector3<int> WorldPosition);
	static int64_t GetChunkKey(Vector3<int> Position);
	static Vector3<int> GetVectorFromKey(int64_t value);

	inline static World* BaseWorld;
private:
};

