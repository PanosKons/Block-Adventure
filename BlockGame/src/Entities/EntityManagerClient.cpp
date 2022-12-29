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
	return *(PlayerClient*)(Players[Networking::credentials->UUID]);
}

void EntityManagerClient::CreateSelf(Credentials& credentials, Player* player)
{
	PlayerClient* playerClient = new PlayerClient(credentials);

	*(Player*)playerClient = *player;

	Players[credentials.UUID] = (Player*)playerClient;
}

void EntityManagerClient::CreatePlayer(int PlayerId, Player* player)
{
	Players[PlayerId] = player;
}
