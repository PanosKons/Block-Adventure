#include "pch.h"
#include "Common/Entities/Credentials.h"
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
		Packet<SizePacket> PacketID = Networking::GetPacketFromServer<SizePacket>();
		switch (PacketID.ExtractPacketData<PACKET_ID>())
		{
		case PACKET_ID::PlayerPosition:
		{
			Packet<ReceivePlayerPosition> packet = Networking::GetPacketFromServer<ReceivePlayerPosition>();
			uint64_t UUID = packet.ExtractPacketData<uint64_t>();
			Vector3<double> Position = packet.ExtractPacketData<Vector3<double>>();
			if(UUID != EntityManagerClient::GetPlayer().credentials.UUID)
				EntityManagerClient::Players[UUID]->Position = Position;
			break;
		}
		case PACKET_ID::PlayerJoin:
		{
			Packet<ReceivePlayerJoin> packet = Networking::GetPacketFromServer<ReceivePlayerJoin>();
			Player* player = new Player(*Networking::credentials);
			*player = packet.ExtractPacketData<Player>();
			EntityManagerClient::Players[player->credentials.UUID] = player;
			INFO("Player with name:", player->credentials.Name, " and UUID:", player->credentials.UUID, " is in game!");
			break;
		}
		case PACKET_ID::ReplaceBlock:
		{
			Packet<ReceiveReplaceBlock> packet = Networking::GetPacketFromServer<ReceiveReplaceBlock>();
			Vector3<int> BlockPosition = packet.ExtractPacketData<Vector3<int>>();
			unsigned short id = packet.ExtractPacketData<unsigned short>();
			Block block = WorldManager::BaseWorld->GetBlock(BlockPosition);
			block.OnBreak((BLOCK_ID)id);
			break;
		}
		case PACKET_ID::NewChunk:
		{
			Packet<ReceiveNewChunk> packet = Networking::GetPacketFromServer<ReceiveNewChunk>();
			Vector3<int> ChunkPosition = packet.ExtractPacketData<Vector3<int>>();
			Packet<ChunkPacketSize> chunkPacket = Networking::GetPacketFromServer<ChunkPacketSize>();
			BlockArray* blocks = (BlockArray*)chunkPacket.GetPacket();
			WorldManager::BaseWorld->CreateChunk(ChunkPosition, (BlockArray*)chunkPacket.GetPacket());
			WorldManagerClient::RefreshBorderChunks(WorldManager::BaseWorld,ChunkPosition);
			chunkPacket.SetPacket(nullptr);
			break;
		}
		case PACKET_ID::DeleteChunk:
		{
			Packet<ReceiveDeleteChunk> packet = Networking::GetPacketFromServer<ReceiveDeleteChunk>();
			Vector3<int> ChunkPosition = packet.ExtractPacketData<Vector3<int>>();
			WorldManager::BaseWorld->DestroyChunk(ChunkPosition);
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