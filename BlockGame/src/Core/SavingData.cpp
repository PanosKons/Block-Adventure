#include <Engine.h>
#include "SavingData.h"
#include "GameManager.h"
#include "util/StringConvertions.h"
#include <fstream>
#include <array>
#include <filesystem>
#include <iostream>
void SavingData::SaveChunk(Chunk* chunk)
{
	std::ofstream fout;
	fout.open("data/chunks/" + StringConvertions::ToString(chunk->Position.x) + "," + StringConvertions::ToString(chunk->Position.y), std::ios::binary);
	std::array<unsigned short, ChunkSize* ChunkHeight* ChunkSize>* bytes = new std::array<unsigned short, ChunkSize* ChunkHeight* ChunkSize>();
	auto& a = chunk->blocks;
	for (int x = 0; x < ChunkSize; x++)
		for (int y = 0; y < ChunkHeight; y++)
			for (int z = 0; z < ChunkSize; z++)
				(*bytes)[z + y * ChunkSize + x * ChunkHeight * ChunkSize] = (unsigned short)(*a)[x][y][z]->GetBlockId();
	fout.write((const char*)(*bytes).data(), (*bytes).size() * sizeof(unsigned short));
	fout.close();
}
std::array<std::array<std::array<Block*, ChunkSize>, ChunkHeight>, ChunkSize>* SavingData::LoadChunk(Vector2<int> Position)
{
	if (!Activated)
		return nullptr;
	std::array<std::array<std::array<Block*, ChunkSize>, ChunkHeight>, ChunkSize>* blocks = new std::array<std::array<std::array<Block*, ChunkSize>, ChunkHeight>, ChunkSize>();
	std::array<unsigned short, ChunkSize* ChunkHeight* ChunkSize>* bytes = new std::array<unsigned short, ChunkSize* ChunkHeight* ChunkSize>();
	std::ifstream fin;
	const std::string path = "data/chunks/" + StringConvertions::ToString(Position.x) + "," + StringConvertions::ToString(Position.y);
	if (!std::filesystem::exists(path)) return nullptr;
	fin.open(path, std::ios::binary | std::ios::in);
	fin.read((char*)(*bytes).data(), (*bytes).size() * sizeof(unsigned short));
	fin.close();
	unsigned short aa = (*bytes)[16 * 16 * 64];
	unsigned short aaa = (*bytes)[16 * 16 * 64 + 1];
	for (int x = 0; x < ChunkSize; x++)
		for (int y = 0; y < ChunkHeight; y++)
			for (int z = 0; z < ChunkSize; z++)
			{
				(*blocks)[x][y][z] = World::MakeBlock((BLOCK_ID)(*bytes)[z + y * ChunkSize + x * ChunkHeight * ChunkSize]);
				(*blocks)[x][y][z]->Position = { (x + ChunkSize * Position.x),y,z + ChunkSize * Position.y };
			}
	return blocks;
}
std::array<BLOCK_ID, StructureSize* StructureSize* StructureSize>* SavingData::LoadStructure(const char* name)
{
	std::array<BLOCK_ID, StructureSize* StructureSize* StructureSize>* data = new std::array<BLOCK_ID, 32 * 32 * 32>();
	std::ifstream fin;
	const std::string path = "data/structures/" + std::string(name);
	if (!std::filesystem::exists(path)) return nullptr;
	fin.open(path, std::ios::binary | std::ios::in);
	fin.read((char*)(*data).data(), (*data).size() * sizeof(BLOCK_ID));
	fin.close();
	return data;
}
void SavingData::SaveStructure(std::string& name, Structure structure)
{
	std::ofstream fout;
	fout.open("data/structures/" + name, std::ios::binary);
	fout.write((const char*)(*structure.data).data(), (*structure.data).size() * sizeof(BLOCK_ID));
	fout.close();
}

void SavingData::ActivateLoading(bool value)
{
	Activated = value;
}
