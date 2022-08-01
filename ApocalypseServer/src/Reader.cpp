#include "Reader.h"
#include "pch.h"
#include "Common/Math/StringConvertions.h"

void Reader::Init()
{
	if (!std::filesystem::exists("Data"))
		std::filesystem::create_directory("Data");
	if(!std::filesystem::exists("Data/Player"))
		std::filesystem::create_directory("Data/Player");
	if (!std::filesystem::exists("Data/World"))
		std::filesystem::create_directory("Data/World");
}

Player* Reader::ReadPlayer(int PlayerId)
{
	if (!std::filesystem::exists("Data/Player/Player_" + StringConvertions::ToString(PlayerId)))
	{
		std::filesystem::create_directory("Data/Player/Player_" + StringConvertions::ToString(PlayerId));
		Player* player = new Player();

		player->Position = { 0,0,0 };
		player->Hitbox = { 0.6, 1.8 ,0.6 };
		player->Velocity = { 0,0,0 };
		player->MaxHealth = 100.0f;
		player->Health = 100.0f;
		player->Speed = 5.0f;
		player->Pitch = 30.0f;
		player->Yaw = 30.0f;
		player->Grounded = false;

		WritePlayer(player);
		return player;
	}
	else
	{

	}
}
