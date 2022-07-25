#include "pch.h"
#include "WorldManager.h"

Block WorldManager::GetBlock(BlockData* data, Vector3<int> WorldPosition)
{
    return Block(WorldPosition,data);
}
int64_t WorldManager::GetChunkKey(short x, short y, short z)
{
	int64_t data = 0;
	short* p = (short*)&data;
	(*p) = x;
	p++;
	(*p) = y;
	p++;
	(*p) = z;
	return data;
}
Vector3<int> WorldManager::GetVectorFromKey(int64_t value)
{
	Vector3<int> v;
	short* p = (short*)&value;
	v.x = *p;
	p++;
	v.y = *p;
	p++;
	v.z = *p;
	return v;
}
