#include "pch.h"
#include "Block.h"
#include "World/WorldManager.h"
void Block::StateChanged()
{
	std::array<Block, 6> blocks =
	{
		WorldManager::BaseWorld->GetBlock({ Position.x + 1, Position.y, Position.z }),
		WorldManager::BaseWorld->GetBlock({ Position.x - 1, Position.y, Position.z }),
		WorldManager::BaseWorld->GetBlock({ Position.x, Position.y, Position.z + 1 }),
		WorldManager::BaseWorld->GetBlock({ Position.x, Position.y, Position.z - 1 }),
		WorldManager::BaseWorld->GetBlock({ Position.x, Position.y + 1, Position.z }),
		WorldManager::BaseWorld->GetBlock({ Position.x, Position.y - 1, Position.z })
	};
	for (unsigned int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].data != nullptr)
			if (blocks[i].GetBlockId() != BLOCK_ID::Air)
			{
				blocks[i].Update();
			}
	}
}
bool Block::IsBlockSolid(Vector3<int> Position)
{
	
	Block block = WorldManager::BaseWorld->GetBlock({ Position.x,Position.y,Position.z });
	if (block.data != nullptr)
		return block.GetBlockId() != BLOCK_ID::Air;
	return true;
	
}
void Block::OnBreak(BLOCK_ID id)
{
	std::array<char, sizeof(Vector3<int>) + sizeof(BLOCK_ID)> buffer = std::array<char, sizeof(Vector3<int>) + sizeof(BLOCK_ID)>();
	int* p = (int*)buffer.data();
	*(Vector3<int>*)p = Position;
	*(p + sizeof(Vector3<int>) / sizeof(int)) = (int)id;
	Networking::SendData(PACKET_ID::BreakBlock, buffer.data(), sizeof(Position) + sizeof(BLOCK_ID));
	OnBreakOffline(id);
}
Block::Block()
	:data(nullptr)
{}
bool Block::operator!=(Block& other)
{
	return data != other.data || Position != other.Position;
}
Block::Block(Vector3<int> Position, BlockData * data)
{
	this->Position = Position;
	this->data = data;
}
BLOCK_ID Block::GetBlockId() const
{
	return (BLOCK_ID)data->blockId;
}
bool Block::GetTransparent()
{
	if ((BLOCK_ID)data->blockId == BLOCK_ID::Air || (BLOCK_ID)data->blockId == BLOCK_ID::Water || (BLOCK_ID)data->blockId == BLOCK_ID::Leaves || (BLOCK_ID)data->blockId == BLOCK_ID::Glass)
		return true;
	return false;
}
BlockProperties Block::GetBlockProperties()
{
	switch ((BLOCK_ID)data->blockId)
	{
	case BLOCK_ID::Air:
		return { 0,TOOL::None,0, {INVALID,INVALID,INVALID,INVALID,INVALID,INVALID} };
	case BLOCK_ID::Cobblestone:
		return { 180,TOOL::Pickaxe,0,{3,3,3,3,3,3} };
	case BLOCK_ID::Dirt:
		return { 60,TOOL::Shovel,0,{2,2,2,2,2,2} };
	case BLOCK_ID::DryGrass:
		return { 60,TOOL::Shovel,0,{12,12,12,12,2,10} };
	case BLOCK_ID::Glass:
		return { 60,TOOL::None,0,{7,7,7,7,7,7} };
	case BLOCK_ID::Grass:
		return { 60,TOOL::Shovel,0,{1,1,1,1,2,0} };
	case BLOCK_ID::Iron:
		return { 300,TOOL::Pickaxe,0,{6,6,6,6,6,6} };
	case BLOCK_ID::Leaves:
		return { 5,TOOL::Axe,0,{8,8,8,8,8,8} };
	case BLOCK_ID::Log:
		return { 120,TOOL::Axe,0,{4,4,4,4,5,5} };
	case BLOCK_ID::Water:
		return { 20,TOOL::None,0,{14,14,14,14,14,14} };
	};
	return {};
}
void Block::Update()
{
	if (data->blockId == (unsigned short)BLOCK_ID::Air) return;
	unsigned char oldSides = data->RenderedSides;
	Block blocks[] =
	{
		WorldManager::BaseWorld->GetBlock({ Position.x + 1, Position.y, Position.z }),
		WorldManager::BaseWorld->GetBlock({ Position.x - 1, Position.y, Position.z }),
		WorldManager::BaseWorld->GetBlock({ Position.x, Position.y, Position.z + 1 }),
		WorldManager::BaseWorld->GetBlock({ Position.x, Position.y, Position.z - 1 }),
		WorldManager::BaseWorld->GetBlock({ Position.x, Position.y + 1, Position.z }),
		WorldManager::BaseWorld->GetBlock({ Position.x, Position.y - 1, Position.z })
	};
	if (blocks[0].data != nullptr)
		if (blocks[0].GetTransparent() && blocks[0].GetBlockId() != GetBlockId())
		{
			data->RenderedSides = data->RenderedSides | 2;
		}
		else
		{
			data->RenderedSides = data->RenderedSides & (255 - 2);
		}
	if (blocks[1].data != nullptr)
		if (blocks[1].GetTransparent() && blocks[1].GetBlockId() != GetBlockId())
		{
			data->RenderedSides = data->RenderedSides | 8;
		}
		else
		{
			data->RenderedSides = data->RenderedSides & (255 - 8);
		}
	if (blocks[2].data != nullptr)
		if (blocks[2].GetTransparent() && blocks[2].GetBlockId() != GetBlockId())
		{
			data->RenderedSides = data->RenderedSides | 4;
		}
		else
		{
			data->RenderedSides = data->RenderedSides & (255 - 4);
		}
	if (blocks[3].data != nullptr)
		if (blocks[3].GetTransparent() && blocks[3].GetBlockId() != GetBlockId())
		{
			data->RenderedSides = data->RenderedSides | 1;
		}
		else
		{
			data->RenderedSides = data->RenderedSides & (255 - 1);
		}
	if (blocks[4].data != nullptr)
		if (blocks[4].GetTransparent() && blocks[4].GetBlockId() != GetBlockId())
		{
			data->RenderedSides = data->RenderedSides | 32;
		}
		else
		{
			data->RenderedSides = data->RenderedSides & (255 - 32);
		}
	if (blocks[5].data != nullptr)
		if (blocks[5].GetTransparent() && blocks[5].GetBlockId() != GetBlockId())
		{
			data->RenderedSides = data->RenderedSides | 16;
		}
		else
		{
			data->RenderedSides = data->RenderedSides & (255 - 16);
		}

	if (oldSides != data->RenderedSides)
	{
		Chunk* chunk = WorldManager::BaseWorld->GetChunk(this->Position);
		if (chunk != nullptr) chunk->Changed = true;
	}
}

void Block::OnBreakOffline(BLOCK_ID id)
{
	data->blockId = (unsigned short)id;
	Update();
	StateChanged();
	Chunk* chunk = WorldManager::BaseWorld->GetChunk(this->Position);
	if(chunk != nullptr) chunk->Changed = true;
}
