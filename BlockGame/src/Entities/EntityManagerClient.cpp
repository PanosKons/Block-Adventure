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

void EntityManagerClient::CreateSelf(int PlayerId, Player* player)
{
	PlayerClient* playerClient = new PlayerClient();

	*(Player*)playerClient = *player;

	Players[PlayerId] = (Player*)playerClient;
}

void EntityManagerClient::CreatePlayer(int PlayerId, Player* player)
{
	Players[PlayerId] = player;
}
