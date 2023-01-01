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
BlockProperties& Block::GetBlockProperties(BlockData* data)
{
	return blockProperties[data->blockId];
}
BlockProperties& Block::GetBlockProperties() const
{
	return blockProperties[data->blockId];
}
//bool Block::IsBlockSolid(Vector3<int> Position)
//{
//	Block block = WorldManager::BaseWorld->GetBlock({ Position.x,Position.y,Position.z });
//	if (block.data != nullptr)
//		return blockProperties[block.GetBlockId()].render;
//	return true;
//}