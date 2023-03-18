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
#include "../lua/LuaManager.h"
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
			switch (GetDataFromClient<Packet>(credentials))
			{
				case Packet::PlayerPosition:
				{
					PlayerPositionData data = GetDataFromClient<PlayerPositionData>(credentials);
					EntityManagerServer::GetPlayer(data.UUID)->Position = data.playerPosition;

					SendDataAllExceptClient(credentials.UUID, Packet::PlayerPosition, data);
					break;
				}
				case Packet::PlayerRotation:
				{
					PlayerRotationData data = GetDataFromClient<PlayerRotationData>(credentials);

					EntityManagerServer::GetPlayer(data.UUID)->Pitch = data.playerRotation.x;
					EntityManagerServer::GetPlayer(data.UUID)->Yaw = data.playerRotation.y;

					SendDataAllExceptClient(credentials.UUID, Packet::PlayerRotation, data);
					break;
				}
				case Packet::SelectSlot:
				{
					SelectSlotData data = GetDataFromClient<SelectSlotData>(credentials);
					EntityManagerServer::GetPlayer(credentials.UUID)->ActiveSlot = data.ActiveSlot;
					break;
				}
				case Packet::MouseState:
				{
					auto data = GetDataFromClient<MouseStateData>(credentials);
					LuaManager::MouseEvent(credentials.UUID, data.LeftMouse, data.RightMouse, data.MiddleMouse);
				}
			}
		}
	}
	void Shutdown()
	{
		WSACleanup();
	}
	Credentials GetCredentials(SOCKET* socket)
	{
		Credentials credentials;
		int TotalReceivedBytes = 0;
		do
		{
			int ReceivedBytes = recv(*socket, (char*)&credentials + TotalReceivedBytes, sizeof(Credentials) - TotalReceivedBytes, 0);
			TotalReceivedBytes += ReceivedBytes;
		} while (TotalReceivedBytes != sizeof(Credentials));

		return credentials;
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
			Credentials credentials = GetCredentials(client);

			std::mutex mutex;
			mutex.lock();
			sockets[credentials.UUID] = client;
			mutex.unlock();

			EntityManagerServer::CreatePlayer(credentials);

			SendDataAllClients(Packet::PlayerJoin, *EntityManagerServer::GetPlayer(credentials.UUID));

			StartData data;
			data.player = *EntityManagerServer::GetPlayer(credentials.UUID);
			data.BlockCount = Block::GetBlockCount();
			data.ItemCount = Item::GetItemCount();
			data.WorldGen = {
				Block::FillerBlock,
				Block::UndergroundBlock,
				Block::DirtBlock,
				Block::DryTopBlock,
				Block::WetTopBlock,
				Block::DeadTopBlock,
				Block::StoneTopBlock,
				Block::OreBlock,
			};
			SendDataToClient(credentials.UUID,Packet::None, data);

			for (int i = 0; i < Block::GetBlockCount(); i++)
			{
				SendDataToClient(credentials.UUID, Packet::None, Block::blockProperties[i]);
			}
			for (int i = 0; i < Item::GetItemCount(); i++)
			{
				SendDataToClient(credentials.UUID, Packet::None, Item::itemProperties[i]);
			}
			EntityManagerServer::GetPlayer(credentials.UUID)->IsReadyToReceivePackets = true;

			for (auto&[UUID, player] : EntityManagerServer::Players)
			{
				if (UUID != credentials.UUID)
				{
					SendDataToClient(credentials.UUID, Packet::PlayerJoin, *player);
				}
			}
			INFO("Client with name: ", credentials.Name, " and UUID:", credentials.UUID, " connected to the server");
			std::thread work(HandleClientPacket, credentials);
			work.detach();
		}
	}
}
