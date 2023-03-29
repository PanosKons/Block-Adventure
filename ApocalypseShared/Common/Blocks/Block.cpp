#include "pch.h"
#include "Block.h"
#include "World/WorldManager.h"
#include "Logger.h"
Block::Block()
	:Position(0),data(nullptr){}
Block::Block(Vector3<int> Position, BlockData* data)
	:Position(Position),data(data){}
Block::~Block(){}

//BlockType Block::GetBlockType(std::string&& key)
//{
//	for (size_t i = 0; i < Block::blockProperties.size(); i++)
//	{
//		if (key == std::string(Block::blockProperties[i].name.data())) return (BlockType)i;
//	}
//	ERR("BlockId:", key, " is invalid");
//}

//BlockType Block::GetBlockType(std::string& key)
//{
//	for (size_t i = 0; i < Block::blockProperties.size(); i++)
//	{
//		if (key == std::string(Block::blockProperties[i].name.data())) return (BlockType)i;
//	}
//	ERR("BlockId:", key, " is invalid");
//}
bool Block::operator!=(Block& other)
{
	return data != other.data || Position != other.Position;
}
bool Block::IsValid()
{
	return data;
}
BlockType Block::GetBlockId() const
{
	return data->blockId;
}
BlockProperties& Block::GetBlockProperties(BlockType blocktype)
{
	return blockProperties[blocktype];
}
BlockProperties& Block::GetBlockProperties() const
{
	ASSERT(data, "Invalid block");
	ASSERT(data->blockId < blockProperties.size(), "Invalid blockId");
	return blockProperties[data->blockId];
}

ItemProperties& Item::GetItemProperties(int itemType)
{
	return itemProperties[itemType];
}
//ItemType Item::GetItemType(std::string&& key)
//{
//	for (size_t i = 0; i < Item::itemProperties.size(); i++)
//	{
//		if (key == std::string(Item::itemProperties[i].name.data())) return (ItemType)i;
//	}
//	ERR("ItemId:", key, " is invalid");
//}

//ItemType Item::GetItemType(std::string& key)
//{
//	for (size_t i = 0; i < Item::itemProperties.size(); i++)
//	{
//		if (key == std::string(Item::itemProperties[i].name.data())) return (ItemType)i;
//	}
//	ERR("ItemId:", key, " is invalid");
//}