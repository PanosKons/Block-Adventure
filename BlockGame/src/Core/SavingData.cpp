#include "pch.h"
#include "SavingData.h"
#include "GameManager.h"
#include "Common/Math/StringConvertions.h"

void SavingData::SaveChunk(Vector3<int> Position, std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks)
{
	std::ofstream fout;
	fout.open("data/chunks/" + StringConvertions::ToString(Position.x) + "," + StringConvertions::ToString(Position.y) + "," + StringConvertions::ToString(Position.z) + ".chunk", std::ios::binary);
	fout.write((const char*)(*blocks).data(), ChunkSize* ChunkSize* ChunkSize * sizeof(BlockData));
	fout.close();
}
void SavingData::LoadChunk(Vector3<int> Position, std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>** blocks)
{
	const std::string path = "data/chunks/" + StringConvertions::ToString(Position.x) + "," + StringConvertions::ToString(Position.y) + "," + StringConvertions::ToString(Position.z) + ".chunk";
	if (!std::filesystem::exists(path)) return;
	std::ifstream fin;
	fin.open(path, std::ios::binary | std::ios::in);
	*blocks = new std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>();
	fin.read((char*)(**blocks).data(), ChunkSize * ChunkSize * ChunkSize * sizeof(BlockData));
	fin.close();
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
void SavingData::SavePlayer(Player* player)
{
	std::array<float, 5> data =
	{
		(float)player->Position.x,
		(float)player->Position.y,
		(float)player->Position.z,
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
