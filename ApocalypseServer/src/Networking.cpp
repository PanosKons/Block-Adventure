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
#include "Networking.h"

namespace Networking {
	inline static std::unordered_map<uint64_t, SOCKET*> sockets;
	void HandleClientPacket(Credentials credentials)
	{
		INFO("Thread with id:", std::this_thread::get_id(), " started!");
		while (true)
		{
			Packet<DefaultPacketSize> packet = GetPacketFromClient<DefaultPacketSize>(credentials);
			switch (packet.ExtractPacketData<PACKET_ID>())
			{
				case PACKET_ID::PlayerPosition:
				{
					Vector3<double> PlayerPosition = packet.ExtractPacketData<Vector3<double>>();
					EntityManagerServer::GetPlayer(credentials.UUID)->Position = PlayerPosition;
					//Packet<DefaultPacketSize> sPacket;
					//sPacket.InitMemory();
					//sPacket.AddPacketData<PACKET_ID>(PACKET_ID::PlayerPosition);
					//sPacket.AddPacketData<uint64_t>(credentials.UUID);
					//sPacket.AddPacketData<Vector3<double>>(PlayerPosition);
					//SendAllClients(sPacket);
					//sPacket.DeletePacket();
				}
				case PACKET_ID::BreakBlock:
				{
				}
			}
			packet.DeletePacket();
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

			Packet<StartPacketSize> StartPacket;
			StartPacket.InitMemory();
			StartPacket.AddPacketData<Player>(*EntityManagerServer::GetPlayer(credentials.UUID));
			SendPacketToClient(credentials, StartPacket);

			INFO("Client with name: ", credentials.Name, " and UUID:", credentials.UUID, " connected to the server");

			std::thread work(HandleClientPacket, credentials);
			work.detach();
		}
	}
}
