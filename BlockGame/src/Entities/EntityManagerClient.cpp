#include "EntityManagerClient.h"
#include "Common/Networking/Packet.h"
#include "Networking/Networking.h"

void EntityManagerClient::Tick()
{
	Packet<DefaultPacketSize> packet;
	packet.InitMemory();
	packet.AddPacketData<PACKET_ID>(PACKET_ID::PlayerPosition);
	packet.AddPacketData<Vector3<double>>(GetPlayer().Position);
	Networking::SendPacketToServer(packet);
}

PlayerClient& EntityManagerClient::GetPlayer()
{
	return *(PlayerClient*)(Players[Networking::Player_id]);
}

void EntityManagerClient::CreateSelf(int PlayerId)
{
	PlayerClient* player = new PlayerClient();

	player->Position = { 0,0,0 };
	player->Hitbox = { 0.6, 1.8 ,0.6 };
	player->Velocity = { 0,0,0 };
	player->MaxHealth = 100.0f;
	player->Health = 100.0f;
	player->Speed = 5.0f;
	player->Pitch = 30.0f;
	player->Yaw = 30.0f;
	player->Grounded = false;

	Players[PlayerId] = (Player*)player;
}