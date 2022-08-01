#pragma once
#include "pch.h"
#include "Chunk.h"
#include "../Blocks.h"
#include "Structure.h"
#include "Player.h"
class SavingData
{
public:
	static void SaveChunk(Vector3<int> Position, std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks);
	static void LoadChunk(Vector3<int> Position, std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>** blocks);
	static Structure* LoadStructure(const char* path);
	static void SaveStructure(std::string& name, Structure structure);
	static void SavePlayer(Player* player);
	static void LoadPlayer(Player* player);
private:
};
