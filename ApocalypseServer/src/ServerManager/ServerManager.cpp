#include "ServerManager.h"
#include "pch.h"
#include "Networking.h"
#include "Common/World/World.h"
#include "Server/Server.h"
#include "Common/World/WorldManager.h"
#include "EntityManagerServer.h"
#include "Common/Entities/Player/Player.h"
#include "Logger.h"

static std::thread* ListeningThread;

void ServerManager::Start()
{
	INFO("Starting server...");
	EntityManagerServer::Start();
	WorldManager::BaseWorld = new World();
	ListeningThread = new std::thread(Networking::ListenForClients);
}

void ServerManager::Tick()
{
	for (auto&[UUID, player] : EntityManagerServer::Players)
	{
		static int RenderDistance = 1;
		Vector3<int> ChunkPosition = Vector::IntVector(player->Position) / ChunkSize;
		for (int x = ChunkPosition.x - RenderDistance; x <= ChunkPosition.x + RenderDistance; x++)
		{
			for (int y = ChunkPosition.y - RenderDistance; y <= ChunkPosition.y + RenderDistance; y++)
			{
				for (int z = ChunkPosition.z - RenderDistance; z <= ChunkPosition.z + RenderDistance; z++)
				{
					Vector3<int> NewChunkPosition = {x,y,z};
					if (WorldManager::BaseWorld->GetChunkMap()->find(WorldManager::GetChunkKey(NewChunkPosition)) == WorldManager::BaseWorld->GetChunkMap()->end())
					{
						WorldManager::BaseWorld->CreateChunk(NewChunkPosition, WorldManager::GenerateChunk(NewChunkPosition));
						WorldManager::BaseWorld->GetChunkDirect(NewChunkPosition)->Refresh();
						{
							Packet<DefaultPacketSize> packet;
							packet.InitMemory();
							packet.AddPacketData(PACKET_ID::NewChunk);
							packet.AddPacketData(NewChunkPosition);
							Networking::SendPacketToClient(player->credentials, packet);
							packet.DeletePacket();
						}
						{
							Packet<ChunkPacketSize> sPacket;
							sPacket.SetPacket((std::array<char, ChunkPacketSize>*)WorldManager::BaseWorld->GetChunkDirect(NewChunkPosition)->GetBlocks());
							Networking::SendPacketToClient(player->credentials, sPacket);
							sPacket.SetPacket(nullptr);
						}
					}
				}
			}
		}
	}
}

void ServerManager::Shutdown()
{
	INFO("Stopping server...");
	EntityManagerServer::ShutDown();
	ListeningThread->join();
	delete ListeningThread;
	delete WorldManager::BaseWorld;
}
