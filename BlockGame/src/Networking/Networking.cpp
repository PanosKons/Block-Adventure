#include "pch.h"
#include "Networking.h"
#include "Common/Math/Vector.h"
#include "GameManager.h"
#include "Client.h"
#include "Common/Entities/EntityManager.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32")
static SOCKET clientSocket = INVALID_SOCKET;
static int Player_id = -1;
void HandleMessage()
{
	while (true)
	{
		std::array<char, defaultsize> buffer = std::array<char, defaultsize>();
		recv(clientSocket, buffer.data(), buffer.size(), 0);
		if (ShuttingDown == true) return;
		int* p = (int*)buffer.data();
		PACKET_ID id = *(PACKET_ID*)p;
		switch (id)
		{
		case PACKET_ID::PlayerPosition:
		{
			int other_player_id = *(p + 1);
			Vector3<double>* vector = (Vector3<double>*)(buffer.data() + sizeof(int) * 2);
			EntityManager::UpdatePlayer(other_player_id, *vector);
			break;
		}
		case PACKET_ID::BreakBlock:
		{
			int other_player_id = *(p + 1);
			Vector3<int>* vector = (Vector3<int>*)(buffer.data() + sizeof(int) * 2);
			BLOCK_ID* blockid = (BLOCK_ID*)(buffer.data() + sizeof(int) * 2 + sizeof(Vector3<int>));
			GameManager::Overworld->GetBlock(*vector).OnBreakOffline(*blockid);
			break;
		}
		case PACKET_ID::SendChunk:
		{
			Vector3<int>* vector = (Vector3<int>*)(buffer.data() + sizeof(int));
			std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks = new std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>();
			send(clientSocket, buffer.data(), buffer.size(), 0);
			recv(clientSocket, (char*)blocks->data(), ChunkSize* ChunkSize* ChunkSize*sizeof(BlockData), 0);
			GameManager::Overworld->MakeNewChunk(*vector,blocks);
			GameManager::Overworld->SubmitChunkChanges();
			send(clientSocket, buffer.data(), buffer.size(), 0);
			break;
		}
		case PACKET_ID::DeleteChunk:
		{
			Vector3<int>* vector = (Vector3<int>*)(buffer.data() + sizeof(int));
			GameManager::Overworld->DestroyChunk(*vector);
			break;
		}
		}
	}
}
void Networking::SendData(PACKET_ID packet_id, char* data, int sizebytes)
{
}
void Networking::Connect()
{
	//Start dll
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		std::cout << "fail" << std::endl;
	}

	//Make a socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "error" << std::endl;
	}
	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, Client::ip.c_str(), &service.sin_addr.s_addr);
	service.sin_port = htons(Client::port);
	if (connect(clientSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		std::cout << "error" << std::endl;
	}
	char buffer[sizeof(int)];
	recv(clientSocket, buffer, sizeof(int), 0);
	Player_id = *(int*)buffer;
	std::cout << "Connected to the server with id: " << Player_id << std::endl;
	std::thread worker(HandleMessage);
	worker.detach();
}
void Networking::ShutDown()
{
	closesocket(clientSocket);
	WSACleanup();
}