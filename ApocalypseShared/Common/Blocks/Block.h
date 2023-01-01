#pragma once
#include "Common/Math/Vector.h"
#include "Common/Entities/Inventory/ItemStack.h"

constexpr int MaxIdLength = 32;
typedef unsigned short BlockType;

struct Tool {
	int ToolId;
	int ToolMinimumLevel;
};
struct BlockProperties //Each block is paired with some properties at global variables
{
	unsigned int hardness;
	Tool tool;
	std::array<unsigned char, 6> textureSides;
	bool render;
	bool transparent;
	std::array<char, MaxIdLength> name;
};
struct BlockData
{
	BlockType blockId;
	unsigned char RenderedSides = 64;
	//unsigned char BlockProperties;
};

#define INVALID 255
class Block
{
public:
	Block();
	Block(Vector3<int> Position, BlockData* data);
	~Block();

	BlockType GetBlockId() const;
	BlockProperties& GetBlockProperties() const;
	static BlockProperties& GetBlockProperties(BlockData* data);
	bool operator!=(Block& other);
	bool IsValid();
	static BlockType GetBlockType(std::string&& key);
	static BlockType GetBlockType(std::string& key);
	inline static int GetBlockCount() { return (int)blockProperties.size(); }
	inline static int GetToolCount() { return (int)toolTypes.size(); }

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
	//TEMPORARY (UNTIL NETWORKING BECOMES A CLASS)
	inline static std::vector<BlockProperties> blockProperties;
private:
	BlockData* data;
	inline static std::vector<std::array<char, MaxIdLength>> toolTypes;

	friend class WorldManager;
	friend class LuaManager;
	friend class RendererClient;
	friend class Networking;
};