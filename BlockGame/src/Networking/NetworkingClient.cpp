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
		Packet<SizePacket> PacketID = NetworkingClient::GetPacketFromServer<SizePacket>();
		switch (PacketID.ExtractPacketData<PACKET_ID>())
		{
		case PACKET_ID::PlayerPosition:
		{
			Packet<ReceivePlayerPosition> packet = NetworkingClient::GetPacketFromServer<ReceivePlayerPosition>();
			uint64_t UUID = packet.ExtractPacketData<uint64_t>();
			Vector3<double> Position = packet.ExtractPacketData<Vector3<double>>();
			if (UUID != EntityManagerClient::GetPlayer().credentials.UUID)
				EntityManagerClient::Players[UUID]->Position = Position;
			break;
		}
		case PACKET_ID::PlayerRotation:
		{
			Packet<ReceivePlayerRotation> packet = NetworkingClient::GetPacketFromServer<ReceivePlayerRotation>();
			uint64_t UUID = packet.ExtractPacketData<uint64_t>();
			Vector2<float> Rotation = packet.ExtractPacketData<Vector2<float>>();
			if (UUID != EntityManagerClient::GetPlayer().credentials.UUID)
			{
				EntityManagerClient::Players[UUID]->Pitch = Rotation.x;
				EntityManagerClient::Players[UUID]->Yaw = Rotation.y;
			}
			break;
		}
		case PACKET_ID::PlayerJoin:
		{
			Packet<ReceivePlayerJoin> packet = NetworkingClient::GetPacketFromServer<ReceivePlayerJoin>();
			Player* player = new Player(*NetworkingClient::credentials);
			*player = packet.ExtractPacketData<Player>();
			EntityManagerClient::Players[player->credentials.UUID] = player;
			INFO("Player with name:", player->credentials.Name, " and UUID:", player->credentials.UUID, " is in game!");
			break;
		}
		case PACKET_ID::ReplaceBlock:
		{
			Packet<ReceiveReplaceBlock> packet = NetworkingClient::GetPacketFromServer<ReceiveReplaceBlock>();
			Vector3<int> BlockPosition = packet.ExtractPacketData<Vector3<int>>();
			BlockType id = packet.ExtractPacketData<BlockType>();
			Block block = WorldManager::BaseWorld->GetBlock(BlockPosition);
			WorldManager::ReplaceBlock(block, id);
			break;
		}
		case PACKET_ID::NewChunk:
		{
			Packet<ReceiveNewChunk> packet = NetworkingClient::GetPacketFromServer<ReceiveNewChunk>();
			Vector3<int> ChunkPosition = packet.ExtractPacketData<Vector3<int>>();
			Packet<ChunkPacketSize> chunkPacket = NetworkingClient::GetPacketFromServer<ChunkPacketSize>();
			BlockArray* blocks = (BlockArray*)chunkPacket.GetPacket();
			WorldManager::BaseWorld->CreateChunk(ChunkPosition, (BlockArray*)chunkPacket.GetPacket());
			WorldManagerClient::RefreshBorderChunks(WorldManager::BaseWorld, ChunkPosition);
			chunkPacket.SetPacket(nullptr);
			break;
		}
		case PACKET_ID::DeleteChunk:
		{
			Packet<ReceiveDeleteChunk> packet = NetworkingClient::GetPacketFromServer<ReceiveDeleteChunk>();
			Vector3<int> ChunkPosition = packet.ExtractPacketData<Vector3<int>>();
			WorldManager::BaseWorld->DestroyChunk(ChunkPosition);
			break;
		}
		case PACKET_ID::BlockInteract:
		{
			Packet<ReceiveBlockInteract> packet = NetworkingClient::GetPacketFromServer<ReceiveBlockInteract>();
			uint64_t UUID = packet.ExtractPacketData<uint64_t>();
			Vector3<int> BlockPosition = packet.ExtractPacketData<Vector3<int>>();
			BlockInteractState state = packet.ExtractPacketData<BlockInteractState>();
			float TimeToBreak = packet.ExtractPacketData<float>();
			if (state == BlockInteractState::StartedBreaking)
			{
				EntityManager::GetPlayer(UUID)->BreakingBlockPosition = BlockPosition;
				EntityManager::GetPlayer(UUID)->IsBreakingBlock = true;
				EntityManager::GetPlayer(UUID)->TimeToBreak = TimeToBreak;
			}
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
	Packet<CredentialsPacketSize> CredentialsPacket;
	CredentialsPacket.InitMemory();
	CredentialsPacket.AddPacketData<Credentials>(*credentials);
	NetworkingClient::SendPacketToServer(CredentialsPacket);

	//Receive Player data
	Packet<StartPacketSize> StartPacket;
	StartPacket = NetworkingClient::GetPacketFromServer<StartPacketSize>();
	Player player = StartPacket.ExtractPacketData<Player>();
	int BlockCount = StartPacket.ExtractPacketData<int>();
	int ItemCount = StartPacket.ExtractPacketData<int>();
	Block::FillerBlock = StartPacket.ExtractPacketData<int>();
	Block::UndergroundBlock = StartPacket.ExtractPacketData<int>();
	Block::DirtBlock = StartPacket.ExtractPacketData<int>();
	Block::DryTopBlock = StartPacket.ExtractPacketData<int>();
	Block::WetTopBlock = StartPacket.ExtractPacketData<int>();
	Block::DeadTopBlock = StartPacket.ExtractPacketData<int>();
	Block::StoneTopBlock = StartPacket.ExtractPacketData<int>();
	Block::OreBlock = StartPacket.ExtractPacketData<int>();

	for (int i = 0; i < BlockCount; i++)
	{
		Packet<BlockPropertiesSize> BlockPacket;
		BlockPacket = NetworkingClient::GetPacketFromServer<BlockPropertiesSize>();
		Block::blockProperties.push_back(BlockPacket.ExtractPacketData<BlockProperties>());
	}
	for (int i = 0; i < ItemCount; i++)
	{
		Packet<ItemPropertiesSize> ItemPacket;
		ItemPacket = NetworkingClient::GetPacketFromServer<ItemPropertiesSize>();
		Item::itemProperties.push_back(ItemPacket.ExtractPacketData<ItemProperties>());
	}

	EntityManagerClient::CreateSelf(*NetworkingClient::credentials, &player);


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