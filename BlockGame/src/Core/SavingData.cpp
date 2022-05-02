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
	fout.open("data/chunks/" + StringConvertions::ToString(chunk->GetPosition().x) + "," + StringConvertions::ToString(chunk->GetPosition().y) + "," + StringConvertions::ToString(chunk->GetPosition().z) + ".chunk", std::ios::binary);
	std::array<unsigned short, ChunkSize* ChunkSize* ChunkSize>* bytes = new std::array<unsigned short, ChunkSize* ChunkSize* ChunkSize>();
	for (int x = 0; x < ChunkSize; x++)
		for (int y = 0; y < ChunkSize; y++)
			for (int z = 0; z < ChunkSize; z++)
				(*bytes)[z + y * ChunkSize + x * ChunkSize * ChunkSize] = (unsigned short)(*chunk->GetBlocks())[x][y][z]->GetBlockId();
	fout.write((const char*)(*bytes).data(), (*bytes).size() * sizeof(unsigned short));
	fout.close();
}
std::array<std::array<std::array<Block*, ChunkSize>, ChunkSize>, ChunkSize>* SavingData::LoadChunk(Vector3<int> Position)
{
	if (!Activated)
		return nullptr;
	std::array<std::array<std::array<Block*, ChunkSize>, ChunkSize>, ChunkSize>* blocks = new std::array<std::array<std::array<Block*, ChunkSize>, ChunkSize>, ChunkSize>();
	std::array<unsigned short, ChunkSize* ChunkSize* ChunkSize>* bytes = new std::array<unsigned short, ChunkSize* ChunkSize* ChunkSize>();
	std::ifstream fin;
	const std::string path = "data/chunks/" + StringConvertions::ToString(Position.x) + "," + StringConvertions::ToString(Position.y) + StringConvertions::ToString(Position.z) + ".chunk";
	if (!std::filesystem::exists(path)) return nullptr;
	fin.open(path, std::ios::binary | std::ios::in);
	fin.read((char*)(*bytes).data(), (*bytes).size() * sizeof(unsigned short));
	fin.close();
	for (int x = 0; x < ChunkSize; x++)
		for (int y = 0; y < ChunkSize; y++)
			for (int z = 0; z < ChunkSize; z++)
			{
				(*blocks)[x][y][z] = World::MakeBlock((BLOCK_ID)(*bytes)[z + y * ChunkSize + x * ChunkSize * ChunkSize]);
				(*blocks)[x][y][z]->Position = { x + ChunkSize * Position.x,y + ChunkSize * Position.y,z + ChunkSize * Position.z };
			}
	return blocks;
}

Structure* SavingData::LoadStructure(const char* name)
{
	Structure* structure = new Structure();
	std::ifstream fin;
	const std::string path = "data/structures/" + std::string(name);
	if (!std::filesystem::exists(path)) return nullptr;
	fin.open(path, std::ios::binary | std::ios::in);
	fin.read((char*)structure, sizeof(Structure));
	fin.close();
	return { structure };
}
void SavingData::SaveStructure(std::string& name, Structure structure)
{
	std::ofstream fout;
	fout.open("data/structures/" + name, std::ios::binary);
	fout.write((const char*)&structure, sizeof(Structure));
	fout.close();
}

void SavingData::ActivateLoading(bool value)
{
	Activated = value;
}
void SavingData::SavePlayer(Player* player)
{
	std::array<float, 5> data =
	{
		player->Position.x,
		player->Position.y,
		player->Position.z,
		player->pitch,
		player->yaw
	};
	std::ofstream fout;
	fout.open("data/player.chunk");
	fout.write((const char*)data.data(), data.size() * sizeof(float));
	fout.close();
}
void SavingData::LoadPlayer(Player* player)
{
	if (!Activated)
		return;
	std::ifstream fin;
	const std::string path = "data/player.chunk";
	if(!std::filesystem::exists(path)) return;
	fin.open(path, std::ios::binary | std::ios::in);
	std::array<float,5> position = {0,0,0};
	fin.read((char*)position.data(), position.size() * sizeof(float));
	player->Position.x = position[0];
	player->Position.y = position[1];
	player->Position.z = position[2];
	player->pitch = position[3];
	player->yaw = position[4];
	fin.close();
}
