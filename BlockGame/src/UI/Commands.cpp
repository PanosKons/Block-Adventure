#include "pch.h"
#include "Commands.h"
#include "GameManager.h"
#include "Common/Math/EngineMath.h"
#include "Common/World/Structure.h"
#include "Common/Entities/Inventory/ItemStack.h"
#include "Common/Entities/Entity.h"
#include "Entities/EntityManagerClient.h"
#include "Common/World/WorldManager.h"
#include "Networking/NetworkingClient.h"
void ReplaceBlockAndNotify(Block block, BlockType blockType)
{
	WorldManager::ReplaceBlock(block, blockType);
	//Notify the server
	{
		Packet<SendReplaceBlock> packet;
		packet.InitMemory();
		packet.AddPacketData(PACKET_ID::ReplaceBlock);
		packet.AddPacketData<Vector3<int>>(block.Position);
		packet.AddPacketData<BlockType>(blockType);
		NetworkingClient::SendPacketToServer(packet);
	}
}
void Commands::ExecuteCommand(const std::string& command)
{
	std::vector<std::string> tokens;
	std::stringstream ss(command);
	std::string token;
	while (ss.good()) {
		ss >> token;
		tokens.push_back(token);
	}
	if (tokens[0] == "/give")
	{
		if (tokens.size() == 4)
		{
			if (tokens[1] == "block")
			{
				int count = std::stoi(tokens[3]);
				ItemStack& stack = EntityManagerClient::GetPlayer().Inventory[EntityManagerClient::GetPlayer().GetFirstAvaiableSlot(ItemStack(ItemStackType::BlockItem,(ItemType)Block::GetBlockType(tokens[2]), count))];
				stack = ItemStack(ItemStackType::BlockItem, (ItemType)Block::GetBlockType(tokens[2]), count);
			}
			else if (tokens[1] == "item")
			{
				int count = std::stoi(tokens[3]);
				ItemStack& stack = EntityManagerClient::GetPlayer().Inventory[EntityManagerClient::GetPlayer().GetFirstAvaiableSlot(ItemStack(ItemStackType::Item, Item::GetItemType(tokens[2]), count))];
				stack = ItemStack(ItemStackType::Item, Item::GetItemType(tokens[2]), count);
			}
		}
	}
	//if (tokens[0] == "/structure")
	//{
	//	if (tokens.size() == 12 || tokens.size() == 6)
	//	{
	//		if (tokens[1] == "save")
	//		{
	//			Vector3<int> Position = { std::stoi(tokens[2]),std::stoi(tokens[3]),std::stoi(tokens[4]) };
	//			Vector3<int> Size = { std::stoi(tokens[5]),std::stoi(tokens[6]),std::stoi(tokens[7]) }; // Relative
	//			Vector3<int> Center = { std::stoi(tokens[8]),std::stoi(tokens[9]),std::stoi(tokens[10]) }; // Relative
	//			Structure str;
	//			for (int x = 0; x < Size.x; x++)
	//			{
	//				for (int y = 0; y < Size.y; y++)
	//				{
	//					for (int z = 0; z < Size.z; z++)
	//					{
	//						str.data[x + y * StructureSize + z * StructureSize * StructureSize] = WorldManager::BaseWorld->GetBlock({ x + Position.x,y + Position.y,z + Position.z }).GetBlockId();
	//					}
	//				}
	//			}
	//			str.Center = Center;
	//			//SavingData::SaveStructure(tokens[11], str);
	//		}
	//		else if (tokens[1] == "load")
	//		{
	//			Vector3<int> Position = { std::stoi(tokens[2]),std::stoi(tokens[3]),std::stoi(tokens[4]) };
	//			//WorldManager::BaseWorld->GetChunk(Position)->SpawnStructure({Position.x % ChunkSize , Position.y % ChunkSize , Position.z % ChunkSize}, tokens[5].c_str());
	//		}
	//	}
	//}
	if (tokens[0] == "/tp")
	{
		if (tokens.size() == 4)
		{
			EntityManagerClient::GetPlayer().Position = { (float)std::stoi(tokens[1]), (float)std::stoi(tokens[2]), (float)std::stoi(tokens[3]) };
		}
	}
	//if (tokens[0] == "/entity")
	//{
	//	if (tokens.size() == 4)
	//	{
	//		Entity* en = new Entity();
	//		//EntityManager::Entities.push_back(en);
	//		en->Position = {(double)std::stoi(tokens[1]), (double)std::stoi(tokens[2]), (double)std::stoi(tokens[3])};
	//	}
	//}
	if (tokens[0] == "/set")
	{
		if (tokens.size() == 2)
		{
			Block block = EntityManagerClient::GetPlayer().GetFacingBlock();
			if (!block.IsValid()) return;
			if (block.GetBlockId() != Block::GetBlockType(tokens[1])) ReplaceBlockAndNotify(block, Block::GetBlockType(tokens[1]));
		}
		else if (tokens.size() == 5)
		{
			Block block = WorldManager::BaseWorld->GetBlock({ std::stoi(tokens[1]), std::stoi(tokens[2]), std::stoi(tokens[3]) });
			if (!block.IsValid()) return;
			if (block.GetBlockId() != Block::GetBlockType(tokens[4])) ReplaceBlockAndNotify(block, Block::GetBlockType(tokens[4]));
		}
		else if (tokens.size() == 8)
		{
			int arr[] =
			{
				std::stoi(tokens[1]),
				std::stoi(tokens[2]),
				std::stoi(tokens[3]),
				std::stoi(tokens[4]),
				std::stoi(tokens[5]),
				std::stoi(tokens[6])
			};
			Vector3<int> start = { Math::Min(arr[0],arr[3]),Math::Min(arr[1],arr[4]) ,Math::Min(arr[2],arr[5]) };
			Vector3<int> end = { Math::Max(arr[0],arr[3]),Math::Max(arr[1],arr[4]) ,Math::Max(arr[2],arr[5]) };
			for (int x = start.x; x <= end.x; x++)
				for (int y = start.y; y <= end.y; y++)
					for (int z = start.z; z <= end.z; z++)
					{
						Block block = WorldManager::BaseWorld->GetBlock({ x, y, z });
						if (!block.IsValid()) return;
						if (block.GetBlockId() != Block::GetBlockType(tokens[7])) ReplaceBlockAndNotify(block, Block::GetBlockType(tokens[7]));
					}
		}
	}
}