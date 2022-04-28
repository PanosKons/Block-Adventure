#pragma once
#include <Engine.h>
#include "Chunk.h"
#include "Blocks.h"
#include "Structure.h"
class SavingData
{
public:
	static void SaveChunk(Chunk* chunk);
	static std::array<std::array<std::array<Block*, ChunkSize>, ChunkHeight>, ChunkSize>* LoadChunk(Vector2<int> Position);
	static std::array<BLOCK_ID, StructureSize* StructureSize* StructureSize>* LoadStructure(const char* path);
	static void SaveStructure(std::string& name, Structure structure);
};
