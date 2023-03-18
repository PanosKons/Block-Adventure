#include "pch.h"
#include "Common/Math/Vector.h"
#include "EntityManagerClient.h"
#include "Common/Networking/Packet.h"
#include "Networking/NetworkingClient.h"

void EntityManagerClient::Tick()
{
	PlayerPositionData data;
	data.playerPosition = EntityManagerClient::GetPlayer().Position;
	data.UUID = NetworkingClient::credentials->UUID;
	NetworkingClient::SendDataToServer(Packet::PlayerPosition,data);
}

PlayerClient& EntityManagerClient::GetPlayer()
{
	return *(PlayerClient*)(Players[NetworkingClient::credentials->UUID]);
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
