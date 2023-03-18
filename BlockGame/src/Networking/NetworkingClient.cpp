#include "pch.h"
#include "Common/Entities/Credentials.h"
#include "NetworkingClient.h"
#include "Common/Math/Vector.h"
#include "GameManager.h"
#include "Client.h"
#include "Entities/EntityManagerClient.h"
#include "Logger.h"
#include "World/WorldManagerClient.h"
void HandleMessage()
{
	while (Client::ShouldStop == false)
	{
		switch (NetworkingClient::GetDataFromServer<Packet>())
		{
		case Packet::PlayerPosition:
		{
			PlayerPositionData data = NetworkingClient::GetDataFromServer<PlayerPositionData>();
			EntityManagerClient::Players[data.UUID]->Position = data.playerPosition;
			WARN("Got player pos data");
			break;
		}
		case Packet::PlayerRotation:
		{
			PlayerRotationData data = NetworkingClient::GetDataFromServer<PlayerRotationData>();
			EntityManagerClient::Players[data.UUID]->Pitch = data.playerRotation.x;
			EntityManagerClient::Players[data.UUID]->Yaw = data.playerRotation.y;
			WARN("Got player rot data");
			break;
		}
		case Packet::PlayerJoin:
		{
			Player* player = new Player(*NetworkingClient::credentials);
			*player = NetworkingClient::GetDataFromServer<Player>();
			EntityManagerClient::Players[player->credentials.UUID] = player;
			INFO("Player with name:", player->credentials.Name, " and UUID:", player->credentials.UUID, " is in game!");
			break;
		}
		case Packet::ReplaceBlock:
		{
			ReplaceBlockData data = NetworkingClient::GetDataFromServer<ReplaceBlockData>();
			Block block = WorldManager::BaseWorld->GetBlock(data.Position);
			WorldManager::ReplaceBlock(block, data.blockType);
			break;
		}
		case Packet::NewChunk:
		{
			Vector3<int> ChunkPosition = NetworkingClient::GetDataFromServer<Vector3<int>>();
			BlockArray* data = NetworkingClient::GetChunkDataFromServer();
			WorldManager::BaseWorld->CreateChunk(ChunkPosition, data);
			WorldManagerClient::RefreshBorderChunks(WorldManager::BaseWorld, ChunkPosition);
			break;
		}
		case Packet::DeleteChunk:
		{
			WorldManager::BaseWorld->DestroyChunk(NetworkingClient::GetDataFromServer<Vector3<int>>());
			break;
		}
		}
	}
}
void NetworkingClient::Connect()
{
	//Start dll
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int result = WSAStartup(wVersionRequested, &wsaData);
	ASSERT(!result, "Networking dll failed to initialize");

	//Make a socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ASSERT(clientSocket, "ClientSocket is invalid");
	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, Client::ip.c_str(), &service.sin_addr.s_addr);
	service.sin_port = htons(Client::port);
	INFO("Connecting to the server...");
	int connectionResult = connect(clientSocket, (SOCKADDR*)&service, sizeof(service));
	ASSERT(!connectionResult, "Failed to connect to the server");

	//Send name and UUID
	NetworkingClient::SendDataToServer(Packet::None,*credentials);

	//Receive Player data
	StartData data = NetworkingClient::GetDataFromServer<StartData>();

	//TEMPORARY
	//TRY TO REMOVE THIS CODE TO SEE IF IT WORKS
	Block::FillerBlock = data.WorldGen[0];
	Block::UndergroundBlock = data.WorldGen[1];
	Block::DirtBlock = data.WorldGen[2];
	Block::DryTopBlock = data.WorldGen[3];
	Block::WetTopBlock = data.WorldGen[4];
	Block::DeadTopBlock = data.WorldGen[5];
	Block::StoneTopBlock = data.WorldGen[6];
	Block::OreBlock = data.WorldGen[7];

	for (int i = 0; i < data.BlockCount; i++)
	{
		Block::blockProperties.push_back(NetworkingClient::GetDataFromServer<BlockProperties>());
	}
	for (int i = 0; i < data.ItemCount; i++)
	{
		Item::itemProperties.push_back(NetworkingClient::GetDataFromServer<ItemProperties>());
	}

	EntityManagerClient::CreateSelf(*NetworkingClient::credentials, &data.player);

	INFO("Connected to the server with UUID: ", NetworkingClient::credentials->UUID);

	std::thread worker(HandleMessage);
	worker.detach();
}
void NetworkingClient::ShutDown()
{
	INFO("Disconnected from the server");
	closesocket(clientSocket);
	WSACleanup();
}