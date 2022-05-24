#pragma once
#include "Math/Vector.h"
#include "../BlockProperties.h"
enum class BLOCK_ID
{
	Air, Cobblestone, Grass, Log, Iron, Dirt, Glass, Leaves, Water, DryGrass
};
struct BlockData
{
	unsigned short blockId;
	unsigned char RenderedSides = 64; // front,right,back,left,bottom,top,renders at all
};
class Block //Base class all blocks derive from
{
public:
	bool operator!=(Block& other);
	Block();
	Block(Vector3<int> Position, BlockData* data);
	BLOCK_ID GetBlockId() const; //Each block overrides this method and returns its own id
	bool GetTransparent();
	BlockProperties GetBlockProperties();
	void Update();
	void OnBreakOffline(BLOCK_ID id);
	void OnBreak(BLOCK_ID id);
	void StateChanged();
	Vector3<int> Position;
	BlockData* data;
};