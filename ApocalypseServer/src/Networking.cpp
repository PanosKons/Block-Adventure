#include "Networking.h"
#include "pch.h"
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
namespace Networking {
	static std::vector<SOCKET*> sockets;
	template<int TSize>
	void SendPacketToClient(unsigned char ClientId, Packet<TSize>& packet)
	{
		send(*sockets[ClientId], packet.GetPacket(), packet.GetPacketSize(), 0);
	}
	template<int TSize>
	void SendAllClients(Packet<TSize>& packet)
	{
		for (int i = 0; i < sockets.size(); i++)
		{
			SendPacketToClient(i, packet);
		}
	}
	template<int TSize>
	Packet<TSize> GetPacketFromClient(unsigned char ClientId)
	{
		Packet<TSize> packet;
		packet.InitMemory();
		recv(*sockets[ClientId], packet.GetPacket(), packet.GetPacketSize(), 0);
		return packet;
	}
	void HandleClientPacket(unsigned char ClientId)
	{
		while (true)
		{
			Packet<DefaultPacketSize> packet = GetPacketFromClient<DefaultPacketSize>(ClientId);
			switch (packet.ExtractPacketData<PACKET_ID>())
			{
				case PACKET_ID::PlayerPosition:
				{
					Packet<DefaultPacketSize> sPacket;
					sPacket.InitMemory();
					sPacket.AddPacketData<PACKET_ID>(PACKET_ID::PlayerPosition);
					sPacket.AddPacketData<int>(ClientId);
					sPacket.AddPacketData<Vector3<double>>(packet.ExtractPacketData<Vector3<double>>());
					SendAllClients(sPacket);
				}
				case PACKET_ID::BreakBlock:
				{
					SendAllClients(packet);
				}
				case PACKET_ID::RequestChunk:
				{
					Vector3<int> ChunkPosition = packet.ExtractPacketData<Vector3<int>>();
					WorldManager::BaseWorld->CreateChunk(ChunkPosition,WorldManager::GenerateChunk(ChunkPosition));
					{
						Packet<DefaultPacketSize> sPacket;
						sPacket.InitMemory();
						sPacket.AddPacketData(PACKET_ID::NewChunk);
						sPacket.AddPacketData(ChunkPosition);
						SendPacketToClient(ClientId, sPacket);
					}
					{
						Packet<ChunkPacketSize> sPacket;
						sPacket.SetPacket((std::array<char,ChunkPacketSize>*)WorldManager::BaseWorld->GetChunkDirect(ChunkPosition)->GetBlocks());
						SendPacketToClient(ClientId, sPacket);
					}
				}
			}
		}
	}
	void Shutdown()
	{
		WSACleanup();
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
			static int ClientId = 0;
			INFO("Listening for clients...");
			SOCKET* client = new SOCKET(accept(serverSocket, nullptr, nullptr));
			ASSERT(*client, "Client failed to connect");

			std::mutex mutex;
			mutex.lock();
			sockets.push_back(client);
			mutex.unlock();

			EntityManagerServer::CreatePlayer(ClientId);

			Packet<StartPacketSize> StartPacket;
			StartPacket.InitMemory();
			StartPacket.AddPacketData<int>(ClientId);
			StartPacket.AddPacketData<Player>(*EntityManagerServer::GetPlayer(ClientId));
			SendPacketToClient(ClientId, StartPacket);

			INFO("Client with id: ", ClientId, " connected to the server");

			std::thread work(HandleClientPacket, ClientId);
			work.detach();
			ClientId++;
		}
	}
}
