#pragma once
#include <Engine.h>
#include "Chunk.h"
#include "../Blocks.h"
#include "Structure.h"
#include "Player.h"
static bool Activated = false;
class SavingData
{
public:
	static void SaveChunk(Chunk* chunk);
	static std::array<std::array<std::array<Block*, ChunkSize>, ChunkSize>, ChunkSize>* LoadChunk(Vector3<int> Position);
	static Structure* LoadStructure(const char* path);
	static void SaveStructure(std::string& name, Structure structure);
	static void ActivateLoading(bool value);
	static void SavePlayer(Player* player);
	static void LoadPlayer(Player* player);
private:
};
