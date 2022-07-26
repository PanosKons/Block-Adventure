#include "pch.h"
#include "Networking.h"
#include "Common/Math/Vector.h"
#include "GameManager.h"
#include "Client.h"
#include "Entities/EntityManagerClient.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32")
void HandleMessage()
{
	while (Client::ShuttingDown == false)
	{
		Packet<DefaultPacketSize> packet = Networking::GetPacketFromServer<DefaultPacketSize>();
		switch (packet.ExtractPacketData<PACKET_ID>())
		{
		case PACKET_ID::PlayerPosition:
		{
			int PlayerId = packet.ExtractPacketData<int>();
			Vector3<double> Position = packet.ExtractPacketData<Vector3<double>>();
			if(PlayerId != Networking::Player_id)
				EntityManagerClient::Players[PlayerId]->Position = Position;
			break;
		}
		case PACKET_ID::BreakBlock:
		{
			break;
		}
		case PACKET_ID::NewChunk:
		{
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