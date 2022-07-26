#include "Networking.h"
#include "pch.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32")
#include "Common/Math/Vector.h"
#include "Common/Blocks/BlockData.h"
#include "Common/Math/Noise.h"
#include "Common/World/WorldManager.h"
#include "Server/Server.h"
#include "Common/World/World.h"
#ifdef _DEBUG
#define ASSERTEXITCODE(x) if(x) __debugbreak();
#define ASSERT(x) if(x == 0 || x == -1) __debugbreak();
#else
#define ASSERTEXITCODE(x) if(x) std::cout << "ERROR" << std::endl;
#define ASSERT(x) if(x == 0 || x == -1) std::cout << "ERROR" << std::endl;
#endif
namespace Networking {
	static std::vector<SOCKET*> sockets;
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
					WorldManager::BaseWorld->CreateChunk(ChunkPosition);
				}
			}
		}
	}
	void Shutdown()
	{
		WSACleanup();
	}
	void Tick()
	{
	}
	void ListenForClients()
	{
		//Start dll
		WSADATA wsaData;
		WORD wVersionRequested = MAKEWORD(2, 2);
		int err = WSAStartup(wVersionRequested, &wsaData);
		ASSERTEXITCODE(err);

		//Make a socket
		SOCKET serverSocket = INVALID_SOCKET;
		serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		ASSERT(serverSocket);

		//Bind socket
		sockaddr_in service;
		service.sin_family = AF_INET;
		InetPton(AF_INET, L"127.0.0.1", &service.sin_addr.s_addr);
		service.sin_port = htons(25555);
		ASSERTEXITCODE(bind(serverSocket, (SOCKADDR*)&service, sizeof(service)));
		//Listen socket
		ASSERTEXITCODE(listen(serverSocket, MAX_PLAYERS));

		//Wait for clients
		while (true)
		{
			static int i = 0;
			SOCKET* client = new SOCKET(accept(serverSocket, nullptr, nullptr));
			ASSERT(*client);
			send((*client), (char*)&i, sizeof(int), 0);
			std::mutex mutex;
			mutex.lock();
			sockets.push_back(client);
			mutex.unlock();
			std::cout << "Client with id: " << i << " connected!" << std::endl;
			std::thread work(HandleClientPacket, client, i);
			work.detach();
			i++;
		}
	}
}
