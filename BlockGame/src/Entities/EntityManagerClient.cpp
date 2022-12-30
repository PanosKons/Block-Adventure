#include "pch.h"
#include "Common/Math/Vector.h"
#include "EntityManagerClient.h"
#include "Common/Networking/Packet.h"
#include "Networking/Networking.h"

void EntityManagerClient::Tick()
{
	Packet<SendPlayerPosition> packet;
	packet.InitMemory();
	packet.AddPacketData<PACKET_ID>(PACKET_ID::PlayerPosition);
	packet.AddPacketData<uint64_t>(Networking::credentials->UUID);
	packet.AddPacketData<Vector3<double>>(EntityManagerClient::GetPlayer().Position);
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
