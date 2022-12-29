#include "pch.h"
#include "Networking.h"
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
		Packet<DefaultPacketSize> packet = Networking::GetPacketFromServer<DefaultPacketSize>();
		switch (packet.ExtractPacketData<PACKET_ID>())
		{
		case PACKET_ID::PlayerPosition:
		{
			int PlayerId = packet.ExtractPacketData<int>();
			Vector3<double> Position = packet.ExtractPacketData<Vector3<double>>();
			//if(PlayerId != Networking::Player_id)
			//	EntityManagerClient::Players[PlayerId]->Position = Position;
			break;
		}
		case PACKET_ID::BreakBlock:
		{
			break;
		}
		case PACKET_ID::NewChunk:
		{
			Vector3<int> ChunkPosition = packet.ExtractPacketData<Vector3<int>>();
			Packet<ChunkPacketSize> chunkPacket = Networking::GetPacketFromServer<ChunkPacketSize>();
			BlockArray* blocks = (BlockArray*)chunkPacket.GetPacket();
			WorldManager::BaseWorld->CreateChunk(ChunkPosition, (BlockArray*)chunkPacket.GetPacket());
			WorldManagerClient::RefreshBorderChunks(WorldManager::BaseWorld,ChunkPosition);
			chunkPacket.SetPacket(nullptr);
			break;
		}
		}
	}
}
void Networking::Connect()
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
	Packet<CredentialsPacketSize> CredentialsPacket;
	CredentialsPacket.InitMemory();
	CredentialsPacket.AddPacketData<Credentials>(*credentials);
	Networking::SendPacketToServer(CredentialsPacket);

	//Receive Player data
	Packet<StartPacketSize> StartPacket;
	StartPacket = Networking::GetPacketFromServer<StartPacketSize>();
	Player player = StartPacket.ExtractPacketData<Player>();
	EntityManagerClient::CreateSelf(*Networking::credentials, &player);

	INFO("Connected to the server with UUID: ", Networking::credentials->UUID);

	std::thread worker(HandleMessage);
	worker.detach();
}
void Networking::ShutDown()
{
	INFO("Disconnected from the server");
	closesocket(clientSocket);
	WSACleanup();
}