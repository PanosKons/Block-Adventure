#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32")
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <sstream>
#ifdef _DEBUG
#define ASSERTEXITCODE(x) if(x) __debugbreak();
#define ASSERT(x) if(x == 0 || x == -1) __debugbreak();
#else
#define ASSERTEXITCODE(x) if(x) std::cout << "ERROR" << std::endl;
#define ASSERT(x) if(x == 0 || x == -1) std::cout << "ERROR" << std::endl;
#endif
static std::vector<SOCKET*> sockets;
static std::vector<char> ids;
void EchoClientMessage(SOCKET* socket, char id)
{
	while (true)
	{
		char buffer[200];
		recv(*socket, buffer, 200, 0);
		std::string str = "<Player: ";
		str.push_back(id);
		str.append("> ");
		str.append(buffer);
		for (SOCKET* client : sockets)
		{
			send(*client, str.data(), 200, 0);
		}
		std::cout << str << std::endl;
	}
}
int main()
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
	ASSERTEXITCODE(listen(serverSocket, 10));

	//Wait for clients
	while (true)
	{
		static int i = 0;
		SOCKET* client = new SOCKET(accept(serverSocket, nullptr, nullptr));
		ASSERT(*client);
		std::stringstream strs;
		strs << i;
		char buffer[200];
		strs >> buffer;
		send((*client), buffer, 200, 0);
		std::mutex mutex;
		mutex.lock();
		sockets.push_back(client);
		ids.push_back(buffer[0]);
		mutex.unlock();
		std::cout << "Client with id: " << buffer[0] << " connected!" << std::endl;
		std::thread work(EchoClientMessage, client, buffer[0]);
		work.detach();
		i++;
	}
	for (SOCKET* socket : sockets)
		closesocket(*socket);
	closesocket(serverSocket);
	WSACleanup();
	return 0;
}