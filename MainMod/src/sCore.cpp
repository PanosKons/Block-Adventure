#include "sCore.h"
#include "sBlock.h"
#include "sItem.h"
#include "sInputAction.h"
namespace Script {
	static WorldGenerationData worldgendata = { .Octaves = 8, .Frequency = 256.0, .YLevelStretch = 96, .BiomeStretch = 40, .Filler = Block::Air, .UnderGround = Block::Stone, .Dirt = Block::Dirt, .DryTop = Block::DryGrass, .WetTop = Block::Grass, .DeadTop = Block::Dirt, .StoneTop = Block::Stone, .Ore = Block::Iron, .Water = Block::Water};
	int Core::Initialize() {
		Block::RegisterBlocks();
		Item::RegisterItems();
		sInputAction::RegisterInputActions();
		return 0;
	}
	WorldGenerationData Core::GetWorldGenerationData() { return worldgendata; }

	std::vector<std::string> split(const std::string& s, char delimiter) {
		std::vector<std::string> tokens;
		size_t start = 0, end;

		while ((end = s.find(delimiter, start)) != std::string::npos) {
			tokens.push_back(s.substr(start, end - start));
			start = end + 1;
		}
		tokens.push_back(s.substr(start));
		return tokens;
	}

	void Core::OnCommand(uint64_t UUID, const Command& command)
	{
		std::string s;
		for (size_t i = 0; i < MaxCommandLength; i++)
		{
			if (i == '\n') break;
			s.push_back(command[i]);
		}
		std::vector<std::string> tokens = split(s, ' ');
		if (tokens.size() == 0) return;
		if (tokens[0] == "/give" && tokens.size() == 4) {
			if(tokens[1] == "block"){

			}
		}
	}
}