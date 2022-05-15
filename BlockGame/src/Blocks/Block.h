#pragma once
#include "Math/Vector.h"
#include "../BlockProperties.h"
enum class BLOCK_ID
{
	Air, Cobblestone, Grass, Log, Iron, Dirt, Glass, Leaves, Water, DryGrass
};
class Block //Base class all blocks derive from
{
public:
	Block();
	virtual BLOCK_ID GetBlockId() = 0; //Each block overrides this method and returns its own id
	virtual BlockProperties GetBlockProperties() = 0;
	virtual void Update();
	void OnBreakOffline(BLOCK_ID id);
	void OnBreak(BLOCK_ID id);
	void StateChanged();
	Vector3<int> Position;
	unsigned char RenderedSides; // front,right,back,left,bottom,top,renders at all
	bool Transparent; //Determine if block is transparent
};