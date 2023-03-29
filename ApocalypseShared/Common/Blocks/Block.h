#pragma once
#include "Common/Math/Vector.h"
#include "Common/Entities/Inventory/ItemStack.h"

typedef unsigned short BlockType;

struct BlockProperties //Each block is paired with some properties at global variables
{
	std::array<unsigned char, 6> textureSides;
	float translucency;
	bool render;
	bool transparent;
};
struct ItemProperties
{
	unsigned char texture;
};
struct BlockData
{
	BlockType blockId;
	unsigned char RenderedSides = 64;
	//unsigned char BlockProperties;
};
class Block
{
public:
	Block();
	Block(Vector3<int> Position, BlockData* data);
	~Block();

	BlockType GetBlockId() const;
	BlockProperties& GetBlockProperties() const;
	static BlockProperties& GetBlockProperties(BlockType blocktype);
	bool operator!=(Block& other);
	bool IsValid();
	//static BlockType GetBlockType(std::string&& key);
	//static BlockType GetBlockType(std::string& key);
	inline static int GetBlockCount() { return (int)blockProperties.size(); }

	Vector3<int> Position;

	//World generation
	inline static BlockType FillerBlock;
	inline static BlockType UndergroundBlock;
	inline static BlockType DirtBlock;
	inline static BlockType DryTopBlock;
	inline static BlockType WetTopBlock;
	inline static BlockType DeadTopBlock;
	inline static BlockType StoneTopBlock;
	inline static BlockType OreBlock;
	//TEMPORARY (UNTIL NETWORKINGSERVER BECOMES A CLASS)
	inline static std::vector<BlockProperties> blockProperties;
private:
	BlockData* data;

	friend class WorldManager;
	friend class LuaManager;
	friend class RendererClient;
	friend class Networking;
};
class Item
{
public:
	static ItemProperties& GetItemProperties(int ItemType);
	inline static int GetItemCount() { return (int)itemProperties.size(); };
	//TEMPORARY (UNTIL NETWORKINGSERVER BECOMES A CLASS)
	inline static std::vector<ItemProperties> itemProperties;
private:
	friend class LuaManager;
};