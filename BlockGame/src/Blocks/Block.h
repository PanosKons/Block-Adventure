#pragma once
#include "BlockData.h"
#include "Math/Vector.h"
#include "../BlockProperties.h"

#define INVALID 255
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