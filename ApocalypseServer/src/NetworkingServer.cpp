#include "pch.h"
#include "NetworkingServer.h"
#include "Common/Math/Vector.h"
#include "Common/Blocks/Block.h"
#include "Common/Math/Noise.h"
#include "Common/World/WorldManager.h"
#include "Server/Server.h"
#include "Common/World/World.h"
#include "EntityManagerServer.h"
#include "Logger.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32")

namespace NetworkingServer {
	inline static std::unordered_map<uint64_t, SOCKET*> sockets;
	void HandleClientPacket(Credentials credentials)
	{
		INFO("Thread with id:", std::this_thread::get_id(), " started!");
		while (true)
		{
			Packet<SizePacket> packetID = GetPacketFromClient<SizePacket>(credentials);
			switch (packetID.ExtractPacketData<PACKET_ID>())
			{
				case PACKET_ID::PlayerPosition:
				{
					Packet<ReceivePlayerPosition> packet = GetPacketFromClient<ReceivePlayerPosition>(credentials);
					uint64_t UUID = packet.ExtractPacketData<uint64_t>();
					Vector3<double> playerPosition = packet.ExtractPacketData<Vector3<double>>();
					EntityManagerServer::GetPlayer(UUID)->Position = playerPosition;

					Packet<SendPlayerPosition> SendPacket;
					SendPacket.InitMemory();
					SendPacket.AddPacketData<PACKET_ID>(PACKET_ID::PlayerPosition);
					SendPacket.AddPacketData<uint64_t>(UUID);
					SendPacket.AddPacketData<Vector3<double>>(playerPosition);
					SendAllExceptClient(credentials, SendPacket);
					break;
				}
				case PACKET_ID::ReplaceBlock:
				{
					Packet<ReceiveReplaceBlock> packet = GetPacketFromClient<ReceiveReplaceBlock>(credentials);
					Vector3<int> BlockPosition = packet.ExtractPacketData<Vector3<int>>();
					unsigned short id = packet.ExtractPacketData<unsigned short>();
					Block block = WorldManager::BaseWorld->GetBlock(BlockPosition);
					WorldManager::ReplaceBlock(block,id);

					Packet<SendReplaceBlock> SendPacket;
					SendPacket.InitMemory();
					SendPacket.AddPacketData<PACKET_ID>(PACKET_ID::ReplaceBlock);
					SendPacket.AddPacketData<Vector3<int>>(BlockPosition);
					SendPacket.AddPacketData<unsigned short>(id);
					SendAllExceptClient(credentials, SendPacket);
					break;
				}
			}
		}
	}
	void Shutdown()
	{
		WSACleanup();
	}
	template<int TSize>
	Packet<TSize> GetPacketFromClient(SOCKET* socket)
	{
		Packet<TSize> packet;
		packet.InitMemory();
		int TotalReceivedBytes = 0;
		do
		{
			int ReceivedBytes = recv(*socket, packet.GetPacket() + TotalReceivedBytes, packet.GetPacketSize() - TotalReceivedBytes, 0);
			TotalReceivedBytes += ReceivedBytes;
		} while (TotalReceivedBytes != packet.GetPacketSize());

		return packet;
	}
	int Send(uint64_t UUID, const char* buffer, int len)
	{
		return send(*sockets[UUID], buffer, len, 0);
	}
	int Receive(uint64_t UUID, char* buf, int len)
	{
		return recv(*sockets[UUID],buf,len, 0);
	}
	void ListenForClients()
	{
		//Start dll
		WSADATA wsaData;
		WORD wVersionRequested = MAKEWORD(2, 2);
		int result = WSAStartup(wVersionRequested, &wsaData);
		ASSERT(!result, "Networking dll failed to initialize");

		//Make a socket
		SOCKET serverSocket = INVALID_SOCKET;
		serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		ASSERT(serverSocket, "ServerSocket is invalid");

		//Bind socket
		sockaddr_in service;
		service.sin_family = AF_INET;
		InetPton(AF_INET, L"127.0.0.1", &service.sin_addr.s_addr);
		service.sin_port = htons(25555);
		int bindResult = bind(serverSocket, (SOCKADDR*)&service, sizeof(service));
		ASSERT(!bindResult, "Failed to bind serverSocket");

		//Listen socket
		int listenResult = listen(serverSocket, MAX_PLAYERS);
		ASSERT(!listenResult, "Failed to configure serverSocket to listen");

		//Wait for clients
		while (!Server::ShouldStop)
		{
			INFO("Listening for clients...");
			SOCKET* client = new SOCKET(accept(serverSocket, nullptr, nullptr));
			ASSERT(*client, "Client failed to connect");

			//Receive name and UUID
			Packet<CredentialsPacketSize> CredentialsPacket = GetPacketFromClient<CredentialsPacketSize>(client);
			Credentials credentials = CredentialsPacket.ExtractPacketData<Credentials>();

			std::mutex mutex;
			mutex.lock();
			sockets[credentials.UUID] = client;
			mutex.unlock();

			EntityManagerServer::CreatePlayer(credentials);

			Packet<SendPlayerJoin> packet;
			packet.InitMemory();
			packet.AddPacketData<PACKET_ID>(PACKET_ID::PlayerJoin);
			packet.AddPacketData<Player>(*EntityManagerServer::GetPlayer(credentials.UUID));
			SendAllExceptClient(credentials,packet);

			Packet<StartPacketSize> StartPacket;
			StartPacket.InitMemory();
			StartPacket.AddPacketData<Player>(*EntityManagerServer::GetPlayer(credentials.UUID));
			StartPacket.AddPacketData<int>(Block::GetBlockCount());
			StartPacket.AddPacketData<int>(Block::GetToolCount());
			StartPacket.AddPacketData<int>(Item::GetItemCount());
			StartPacket.AddPacketData<int>(Block::FillerBlock);
			StartPacket.AddPacketData<int>(Block::UndergroundBlock);
			StartPacket.AddPacketData<int>(Block::DirtBlock);
			StartPacket.AddPacketData<int>(Block::DryTopBlock);
			StartPacket.AddPacketData<int>(Block::WetTopBlock);
			StartPacket.AddPacketData<int>(Block::DeadTopBlock);
			StartPacket.AddPacketData<int>(Block::StoneTopBlock);
			StartPacket.AddPacketData<int>(Block::OreBlock);
			SendPacketToClient(credentials, StartPacket);

			for (int i = 0; i < Block::GetBlockCount(); i++)
			{
				Packet<BlockPropertiesSize> BlockPacket;
				BlockPacket.InitMemory();
				BlockPacket.AddPacketData<BlockProperties>(Block::blockProperties[i]);
				SendPacketToClient(credentials, BlockPacket);
			}
			for (int i = 0; i < Item::GetItemCount(); i++)
			{
				Packet<ItemPropertiesSize> ItemPacket;
				ItemPacket.InitMemory();
				ItemPacket.AddPacketData<ItemProperties>(Item::itemProperties[i]);
				SendPacketToClient(credentials, ItemPacket);
			}

			for (auto&[UUID, player] : EntityManagerServer::Players)
			{
				if (UUID != credentials.UUID)
				{
					Packet<SendPlayerJoin> JoinPacket;
					JoinPacket.InitMemory();
					JoinPacket.AddPacketData<PACKET_ID>(PACKET_ID::PlayerJoin);
					JoinPacket.AddPacketData<Player>(*player);
					SendPacketToClient(credentials, JoinPacket);
				}
			}


			INFO("Client with name: ", credentials.Name, " and UUID:", credentials.UUID, " connected to the server");

			std::thread work(HandleClientPacket, credentials);
			work.detach();
		}
	}
}
