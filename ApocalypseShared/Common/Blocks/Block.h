#pragma once
#include "Common/Math/Vector.h"
#include "Common/Entities/Inventory/ItemStack.h"

typedef unsigned short BlockType;

struct BlockProperties //Each block is paired with some properties at global variables
{
	std::array<unsigned char, 6> textureSides;
	int model;
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
	unsigned char Properties;
};
constexpr int MaxFaceCount = 16;
enum class Direction
{
	Forward, Backward, Right, Left, Up, Down
};
struct Face
{
	Vector3<float> position;
	Vector2<float> size;
	Direction direction;
	unsigned char condition;
	unsigned char textureIndex;
	bool Active = false;
};
struct Model
{
	std::array<Face, MaxFaceCount> Faces;
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
	Model& GetBlockModel() const;
	bool operator!=(Block& other);
	bool IsValid();
	inline static int GetBlockCount() { return (int)blockProperties.size(); }
	inline static int GetBlockModelCount() { return (int)blockModels.size(); }

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
	inline static BlockType WaterBlock;
	//TEMPORARY (UNTIL NETWORKINGSERVER BECOMES A CLASS)
	inline static std::vector<BlockProperties> blockProperties;
	inline static std::vector<Model> blockModels;
private:
	BlockData* data;

	friend class WorldManager;
	friend class RendererClient;
	friend class Networking;
};
class Item
{
public:
	static ItemProperties& GetItemProperties(int ItemType);
	inline static int GetItemCount() { return (int)itemProperties.size(); };
	inline static std::vector<ItemProperties> itemProperties;
};