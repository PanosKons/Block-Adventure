#include "pch.h"
#include "Block.h"
#include "World/WorldManager.h"
#include "Logger.h"
Block::Block()
	:Position(0),data(nullptr){}
Block::Block(Vector3<int> Position, BlockData* data)
	:Position(Position),data(data){}
Block::~Block(){}
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
	return blockProperties[data->blockId];
}

ItemProperties& Item::GetItemProperties(int itemType)
{
	return itemProperties[itemType];
}