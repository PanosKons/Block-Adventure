#pragma once
#include "Common/Math/Vector.h"
#include "Common/Entities/Inventory/ItemStack.h"

enum class TOOL
{
	Pickaxe, Shovel, Axe, None
};
struct BlockProperties //Each block is paired with some properties at global variables
{
	unsigned int hardness;
	TOOL tool;
	unsigned int miningLevel;
	std::array<unsigned char, 6> textureSides;
};
std::array<unsigned char, 6>& GetTexture(int id, TYPE type);

enum class BLOCK_ID
{
	Invalid, Air, Cobblestone, Grass, Log, Iron, Dirt, Glass, Leaves, Water, DryGrass
};
struct BlockData
{
	unsigned short blockId;
	unsigned char RenderedSides = 64; // front,right,back,left,bottom,top,renders at all
};

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
	void OnBreak(BLOCK_ID id);
	void StateChanged();


	Vector3<int> Position;
	BlockData* data;

	static bool IsBlockSolid(Vector3<int> Position);
};