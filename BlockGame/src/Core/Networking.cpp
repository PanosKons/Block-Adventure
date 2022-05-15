#include "Networking.h"
#include "Math/Vector.h"
#include "GlobalVariables.h"
#include <Engine.h>
#include "EntityManager.h"
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
		recv(clientSocket, buffer.data(), defaultsize, 0);
		int* p = (int*)buffer.data();
		PACKET_ID id = *(PACKET_ID*)p;
		int other_player_id = *(p + 1);
		switch (id)
		{
		case PACKET_ID::PlayerPosition:
			Vector3<double>* vector = (Vector3<double>*)(buffer.data() + sizeof(int)*2);
			EntityManager::UpdatePlayer(other_player_id, *vector);
			break;
		}
	}
}
void Networking::SendData(PACKET_ID packet_id, char* data, int sizebytes)
{
	std::array<char, defaultsize> buffer = std::array<char,defaultsize>();
	int* p = (int*)buffer.data();
	*p = (int)packet_id;
	*(p + 1) = Player_id;
	for (int i = 0; i < sizebytes; i++)
	{
		buffer[i + sizeof(int)*2] = *(data + i);
	}
	send(clientSocket, buffer.data(), buffer.size(), 0);
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
	InetPton(AF_INET, ip.c_str(), &service.sin_addr.s_addr);
	service.sin_port = htons(25555);
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