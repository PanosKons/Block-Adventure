#include <Engine.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32")
#include "Serializer.h"
#include "Math/Vector.h"
#include "BlockData.h"
#include "Math/Noise.h"
#ifdef _DEBUG
#define ASSERTEXITCODE(x) if(x) __debugbreak();
#define ASSERT(x) if(x == 0 || x == -1) __debugbreak();
#else
#define ASSERTEXITCODE(x) if(x) std::cout << "ERROR" << std::endl;
#define ASSERT(x) if(x == 0 || x == -1) std::cout << "ERROR" << std::endl;
#endif
constexpr int RenderDistance = 1;
class Player
{
public:
	Vector3<int> CurrentChunkPosition = { -1123, -1432, -1754};
	bool ChangedChunk(Vector3<double> Position, Vector3<int>* ChunkPosi)
	{
		Vector3<int> ChunkPos = { (int)Position.x / ChunkSize ,(int)Position.y / ChunkSize,(int)Position.z / ChunkSize };
		if (ChunkPos != CurrentChunkPosition)
		{
			CurrentChunkPosition = ChunkPos;
			*ChunkPosi = ChunkPos;
			return true;
		}
		return false;
	}
};
static std::vector<SOCKET*> sockets;
static std::unordered_map<int, Player> PlayerMap;
std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* CreateChunk(Vector3<int> ChunkPosition)
{
	std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks = new std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>();
	srand(1);
	std::array<int, ChunkSize* ChunkSize> HeightMap;
	std::array<int, ChunkSize* ChunkSize> BiomeMap;
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int z = 0; z < ChunkSize; z++)
		{
			HeightMap[x + z * ChunkSize] = Noise::GetYLevel(x + ChunkPosition.x * ChunkSize, z + ChunkPosition.z * ChunkSize);
			BiomeMap[x + z * ChunkSize] = Noise::GetBiomeTemperature(x + ChunkPosition.x * ChunkSize, z + ChunkPosition.z * ChunkSize);
		}
	}
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				int ylevel = y + ChunkSize * ChunkPosition.y;
				int level = HeightMap[x + z * ChunkSize];
				if (ylevel > level && ylevel <= 30)
				{
					(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Water;
				}
				else if (ylevel == level)
				{
					if (ylevel < 30)
					{
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Dirt;
					}
					else if (BiomeMap[x + z * ChunkSize] == 0)
					{
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Cobblestone;
					}
					else if (BiomeMap[x + z * ChunkSize] == 1)
					{
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Dirt;
					}
					else if (BiomeMap[x + z * ChunkSize] == 2)
					{
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::DryGrass;
					}
					else
					{
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Grass;
					}
				}
				else if (ylevel + 1 == level)
				{
					(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Dirt;
				}
				else if (ylevel + 2 == level)
				{
					(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Dirt;;
				}
				else if (ylevel < level)
				{
					if (rand() % 50 == 0)
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Iron;
					else
						(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Cobblestone;
				}
				else
				{
					(*blocks)[x][y][z].blockId = (unsigned short)BLOCK_ID::Air;
				}
			}
		}
	}
	return blocks;
}
void SendNewChunks(int Player_id, Vector3<int> ChunkPosition)
{
	int startX = ChunkPosition.x - RenderDistance;
	int startY = ChunkPosition.y - RenderDistance;
	int startZ = ChunkPosition.z - RenderDistance;
	int EndX = ChunkPosition.x + RenderDistance;
	int EndY = ChunkPosition.y + RenderDistance;
	int EndZ = ChunkPosition.z + RenderDistance;
	for (int x = startX; x <= EndX; x++)
	{
		for (int y = startY; y <= EndY; y++)
		{
			for (int z = startZ; z <= EndZ; z++)
			{
				std::array<char, defaultsize> buffer;
				*(int*)buffer.data() = (int)PACKET_ID::SendChunk;
				*(Vector3<int>*)(buffer.data() + sizeof(int)) = {x,y,z};
				send(*sockets[Player_id], buffer.data(), defaultsize, 0);
				std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks = CreateChunk({x,y,z});
				recv(*sockets[Player_id], (char*)buffer.data(), buffer.size(), 0);
				send(*sockets[Player_id], (char*)blocks->data(), ChunkSize * ChunkSize * ChunkSize * sizeof(BlockData), 0);
			}
		}
	}
	std::array<char, defaultsize> buffer;
	recv(*sockets[Player_id], (char*)buffer.data(), buffer.size(),0);

}
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
		{
			Player& p = PlayerMap[Player_id];
			Vector3<double>* vector = (Vector3<double>*)(buffer.data() + sizeof(int) * 2);
			Vector3<int> pos;
			if (p.ChangedChunk(*vector, &pos))
				SendNewChunks(Player_id, pos);
			for (int i = 0; i < sockets.size(); i++)
			{
				if (i != Player_id)
					send(*sockets[i], buffer.data(), defaultsize, 0);
			}
			break;
		}
		case PACKET_ID::BreakBlock:
		{
			for (int i = 0; i < sockets.size(); i++)
			{
				if (i != Player_id)
					send(*sockets[i], buffer.data(), defaultsize, 0);
			}
			break;
		}
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
	InetPton(AF_INET, L"192.168.1.16", &service.sin_addr.s_addr);
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