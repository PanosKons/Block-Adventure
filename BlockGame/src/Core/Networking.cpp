#include "Networking.h"
#include <Engine.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32")
static SOCKET clientSocket = INVALID_SOCKET;
void HandleMessage()
{
	while (true)
	{
		char buffer[200];
		recv(clientSocket, buffer, 200, 0);
		std::cout << buffer << std::endl;
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
	InetPton(AF_INET, L"127.0.0.1", &service.sin_addr.s_addr);
	service.sin_port = htons(25555);
	if (connect(clientSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		std::cout << "error" << std::endl;
	}
	std::cout << "Connected to the server!" << std::endl;
	std::thread worker(HandleMessage);
	worker.detach();
}
void Networking::ShutDown()
{
	closesocket(clientSocket);
	WSACleanup();
}