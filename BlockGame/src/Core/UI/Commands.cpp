#include <Engine.h>
#include "Commands.h"
#include "GameManager.h"
#include "Math/EngineMath.h"
#include "Structure.h"
#include "SavingData.h"
static std::unordered_map<std::string, BLOCK_ID> blockIds = //Pair each block with string
{
	{"air",BLOCK_ID::Air },
	{"cobblestone",BLOCK_ID::Cobblestone },
	{"grass",BLOCK_ID::Grass },
	{"log",BLOCK_ID::Log },
	{"dirt",BLOCK_ID::Dirt },
	{"glass",BLOCK_ID::Glass },
	{"leaves",BLOCK_ID::Leaves },
	{"water",BLOCK_ID::Water },
	{"iron",BLOCK_ID::Iron }
};
void Commands::ExecuteCommand(const std::string& command)
{
	std::vector<std::string> tokens;
	std::stringstream ss(command);
	std::string token;
	while (ss.good()) {
		ss >> token;
		tokens.push_back(token);
	}
	if (tokens[0] == "/chunkload")
	{
		if (tokens.size() == 3)
		{
			Vector2<int> Position = { std::stoi(tokens[1]), std::stoi(tokens[2]) };
			GameManager::Overworld->LoadNewChunk(Position);
		}
	}
	if (tokens[0] == "/give")
	{
		if (tokens.size() == 3)
		{
			ItemStack& stack = GameManager::player->Inventory[GameManager::player->GetFirstAvaiableSlot(blockIds[tokens[1]])];
			stack.id = blockIds[tokens[1]];
			stack.count += std::stoi(tokens[2]);
		}
	}
	if (tokens[0] == "/structure")
	{
		if (tokens.size() == 12 || tokens.size() == 6)
		{
			if (tokens[1] == "save")
			{
				Vector3<int> Position = { std::stoi(tokens[2]),std::stoi(tokens[3]),std::stoi(tokens[4]) };
				Vector3<int> Size = { std::stoi(tokens[5]),std::stoi(tokens[6]),std::stoi(tokens[7]) }; // Relative
				Vector3<int> Center = { std::stoi(tokens[8]),std::stoi(tokens[9]),std::stoi(tokens[10]) }; // Relative
				Structure str;
				for (int x = 0; x < Size.x; x++)
				{
					for (int y = 0; y < Size.y; y++)
					{
						for (int z = 0; z < Size.z; z++)
						{
							str.data[x + y * StructureSize + z * StructureSize * StructureSize] = GameManager::Overworld->GetBlock({ x + Position.x,y + Position.y,z + Position.z })->GetBlockId();
						}
					}
				}
				str.Center = Center;
				SavingData::SaveStructure(tokens[11], str);
			}
			else if (tokens[1] == "load")
			{
				Vector3<int> Position = { std::stoi(tokens[2]),std::stoi(tokens[3]),std::stoi(tokens[4]) };
				GameManager::Overworld->GetChunk(Position)->SpawnStructure({Position.x % ChunkSize , Position.y % ChunkHeight , Position.z % ChunkSize}, tokens[5].c_str(),false);
			}
		}
	}
	if (tokens[0] == "/tp")
	{
		if (tokens.size() == 4)
		{
			GameManager::player->Position = { (float)std::stoi(tokens[1]), (float)std::stoi(tokens[2]), (float)std::stoi(tokens[3]) };
		}
	}
	if (tokens[0] == "/set")
	{
		if (tokens.size() == 2)
		{
			Block* block = GameManager::player->GetFacingBlock();
			if (block == nullptr) return;
			if (block->GetBlockId() != blockIds[tokens[1]]) block->OnBreak(blockIds[tokens[1]]);
		}
		else if (tokens.size() == 5)
		{
			Block* block = GameManager::Overworld->GetBlock({ std::stoi(tokens[1]), std::stoi(tokens[2]), std::stoi(tokens[3]) });
			if (block == nullptr) return;
			if (block->GetBlockId() != blockIds[tokens[4]]) block->OnBreak(blockIds[tokens[4]]);
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
						Block* block = GameManager::Overworld->GetBlock({ x, y, z });
						if (block == nullptr) return;
						if (block->GetBlockId() != blockIds[tokens[7]])block->OnBreak(blockIds[tokens[7]]);
					}
		}
	}
}