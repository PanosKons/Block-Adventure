#include "Commands.h"
#include <iostream>
#include <sstream>
#include <vector>
#include "GameManager.h"
#include <unordered_map>
#include "../Math/EngineMath.h"
static std::unordered_map<std::string, BLOCK_ID> blockIds =
{
	{"AIR",BLOCK_ID::Air },
	{"COBBLESTONE",BLOCK_ID::Cobblestone },
	{"DIRT",BLOCK_ID::Dirt },
	{"GRASS_BLOCK",BLOCK_ID::Grass_block },
	{"SAND",BLOCK_ID::Sand },
	{"LOG",BLOCK_ID::Log },
	{"LEAVES",BLOCK_ID::Leaves },
	{"TNT",BLOCK_ID::Tnt }
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
	if (tokens[0] == "/GIVE")
	{
		if (tokens.size() == 3)
		{
			ItemStack& stack = GameManager::player->Inventory[GameManager::player->GetFirstAvaiableSlot(blockIds[tokens[1]])];
			stack.id = blockIds[tokens[1]];
			stack.count += std::stoi(tokens[2]);
		}
	}
	if (tokens[0] == "/SETBLOCK")
	{
		if (tokens.size() == 5)
		{
			Block* block = GameManager::Overworld->GetBlock({ std::stoi(tokens[1]), std::stoi(tokens[2]), std::stoi(tokens[3]) });
			if (block == nullptr) return;
			block->ChangeState(blockIds[tokens[4]]);
		}
	}
	if (tokens[0] == "/TP")
	{
		if (tokens.size() == 4)
		{
			GameManager::player->Position = { (float)std::stoi(tokens[1]), (float)std::stoi(tokens[2]), (float)std::stoi(tokens[3]) };
		}
	}
	if (tokens[0] == "/FILL")
	{
		if (tokens.size() == 8)
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
						block->ChangeState(blockIds[tokens[7]]);
					}
		}
	}
}