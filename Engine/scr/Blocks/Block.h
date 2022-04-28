#pragma once
#include "../Math/Vector.h"
enum class BLOCK_ID
{
	Air, Cobblestone, Grass, Log, Iron, Dirt
};
class Block
{
public:
	Block();
	virtual BLOCK_ID GetBlockId() = 0;
	virtual void Update();
	virtual void OnBreak(BLOCK_ID id);
	void StateChanged();
	Vector3<int> Position;
	unsigned char RenderedSides = 0; // front,right,back,left,bottom,top,renders at all
	bool Transparent = false;
};