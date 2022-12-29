#include "Reader.h"
#include "pch.h"
#include "Common/Math/StringConvertions.h"
#include "Common/Entities/EntityManager.h"

void Reader::Init()
{
	if (!std::filesystem::exists("Data"))
		std::filesystem::create_directory("Data");
	if(!std::filesystem::exists("Data/Player"))
		std::filesystem::create_directory("Data/Player");
	if (!std::filesystem::exists("Data/World"))
		std::filesystem::create_directory("Data/World");
}

Player* Reader::ReadPlayer(Credentials& credentials)
{
	Player* player = new Player(credentials);
	if (std::filesystem::exists("Data/Player/Player_" + StringConvertions::ToString(credentials.UUID)))
	{
		std::ifstream fin;
		fin.open("Data/Player/Player_" + StringConvertions::ToString(credentials.UUID), std::ios::binary | std::ios::in);
		fin.read((char*)player, sizeof(Player));
		fin.close();
	}
	return player;
}

void Reader::WriteAllPlayers()
{
	for (int i = 0; i < EntityManager::Players.size(); i++)
	{
		std::ofstream fout;
		fout.open("Data/Player/Player_" + StringConvertions::ToString(EntityManager::Players[i]));
		fout.write((const char*)EntityManager::Players[i], sizeof(Player));
		fout.close();
	}
}

BlockArray* Reader::ReadWorldChunk(World* world,Vector3<int> ChunkPosition)
{
	if (std::filesystem::exists("Data/World/chunk_" + StringConvertions::ToString(ChunkPosition.x) + "_" + StringConvertions::ToString(ChunkPosition.y) + "_" + StringConvertions::ToString(ChunkPosition.z)))
	{
		BlockArray* blocks = new BlockArray();
		std::ifstream fin;
		fin.open("Data/World/chunk_" + StringConvertions::ToString(ChunkPosition.x) + "_" + StringConvertions::ToString(ChunkPosition.y) + "_" + StringConvertions::ToString(ChunkPosition.z));
		fin.read((char*)blocks,sizeof(BlockArray));
		fin.close();
		return blocks;
	}
	return nullptr;
}

void Reader::WriteWorld(World* world)
{
	for (auto [key, chunk] : *(world->GetChunkMap()))
	{
		std::ofstream fout;
		fout.open("Data/World/chunk_" + StringConvertions::ToString(chunk->GetPosition().x) + "_" + StringConvertions::ToString(chunk->GetPosition().y) + "_" + StringConvertions::ToString(chunk->GetPosition().z));
		fout.write((const char*)chunk->GetBlocks(), sizeof(BlockArray));
		fout.close();
	}
}
