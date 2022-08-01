#include "pch.h"
#include "WorldManager.h"

Block WorldManager::GetBlock(BlockData* data, Vector3<int> WorldPosition)
{
    return Block(WorldPosition,data);
}
int64_t WorldManager::GetChunkKey(Vector3<int> position)
{
	int64_t data = 0;
	short* p = (short*)&data;
	(*p) = (short)position.x;
	p++;
	(*p) = (short)position.y;
	p++;
	(*p) = (short)position.z;
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
