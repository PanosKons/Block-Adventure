#include <Engine.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32")
#include "Serializer.h"
#include "Math/Vector.h"
#ifdef _DEBUG
#define ASSERTEXITCODE(x) if(x) __debugbreak();
#define ASSERT(x) if(x == 0 || x == -1) __debugbreak();
#else
#define ASSERTEXITCODE(x) if(x) std::cout << "ERROR" << std::endl;
#define ASSERT(x) if(x == 0 || x == -1) std::cout << "ERROR" << std::endl;
#endif
static std::vector<SOCKET*> sockets;
void EchoClientMessage(SOCKET* socket, char id)
{
	while (true)
	{
		std::array<char, defaultsize> buffer;
		recv(*socket, buffer.data(), defaultsize, 0);

		int* p = (int*)buffer.data();
		PACKET_ID id = *(PACKET_ID*)p;
		int Player_id = *(p + 1);
		switch (id)
		{
		case PACKET_ID::PlayerPosition:
			for (int i = 0; i < sockets.size(); i++)
			{
				if (i != Player_id)
					send(*sockets[i], buffer.data(), defaultsize, 0);
			}
			break;
		}
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
	InetPton(AF_INET, L"192.168.1.14", &service.sin_addr.s_addr);
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
		std::thread work(EchoClientMessage, client, i);
		work.detach();
		i++;
	}
	for (SOCKET* socket : sockets)
		closesocket(*socket);
	closesocket(serverSocket);
	WSACleanup();
	return 0;
}