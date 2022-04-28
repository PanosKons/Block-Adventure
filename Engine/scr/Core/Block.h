#pragma once
#include "../Math/Vector.h"
class World;
enum BLOCK_ID
{
	Air, Sand, Grass_block, Dirt, Cobblestone, Log, Leaves, Tnt
};
class Block
{
public:
	unsigned char RenderedSides = 0; // front,right,back,left,bottom,top
	BLOCK_ID id;
	Vector3<int> Transform;
	Block();
	~Block();
	void ChangeState(BLOCK_ID newState);
	void Update();
};