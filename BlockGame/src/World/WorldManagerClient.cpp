#include "WorldManagerClient.h"
#include "Entities/PlayerClient.h"
#include "Entities/EntityManagerClient.h"
#include "Common/Networking/Packet.h"
#include "Networking/Networking.h"
#include "Logger.h"

void WorldManagerClient::RequestNewChunks()
{
	static int RenderDistance = 1;
	Vector3<int> ChunkPosition = Vector::IntVector(EntityManagerClient::GetPlayer().Position) / ChunkSize;
	for (int x = ChunkPosition.x - RenderDistance; x <= ChunkPosition.x + RenderDistance; x++)
	{
		for (int y = ChunkPosition.y - RenderDistance; y <= ChunkPosition.y + RenderDistance; y++)
		{
			for (int z = ChunkPosition.z - RenderDistance; z <= ChunkPosition.z + RenderDistance; z++)
			{
				if (WorldManager::BaseWorld->GetChunkDirect({x,y,z}) == nullptr)
				{
					Packet<DefaultPacketSize> packet;
					packet.InitMemory();
					packet.AddPacketData(PACKET_ID::RequestChunk);
					packet.AddPacketData<Vector3<int>>({ x,y,z });
					Networking::SendPacketToServer(packet);
				}
			}
		}
	}
}

void WorldManagerClient::RefreshBorderChunks(World* world, Vector3<int> ChunkPosition)
{
	Chunk* chunk = world->GetChunkDirect(ChunkPosition);
	if (world->GetChunkMap()->find(WorldManager::GetChunkKey({ chunk->GetPosition().x + 1, chunk->GetPosition().y, chunk->GetPosition().z })) != world->GetChunkMap()->end())
		world->GetChunkMap()->at(WorldManager::GetChunkKey({ chunk->GetPosition().x + 1, chunk->GetPosition().y, chunk->GetPosition().z }))->RefreshBorders();
	if (world->GetChunkMap()->find(WorldManager::GetChunkKey({ chunk->GetPosition().x - 1, chunk->GetPosition().y, chunk->GetPosition().z })) != world->GetChunkMap()->end())
		world->GetChunkMap()->at(WorldManager::GetChunkKey({ chunk->GetPosition().x - 1, chunk->GetPosition().y, chunk->GetPosition().z }))->RefreshBorders();
	if (world->GetChunkMap()->find(WorldManager::GetChunkKey({ chunk->GetPosition().x, chunk->GetPosition().y + 1, chunk->GetPosition().z })) != world->GetChunkMap()->end())
		world->GetChunkMap()->at(WorldManager::GetChunkKey({ chunk->GetPosition().x, chunk->GetPosition().y + 1, chunk->GetPosition().z }))->RefreshBorders();
	if (world->GetChunkMap()->find(WorldManager::GetChunkKey({ chunk->GetPosition().x, chunk->GetPosition().y - 1, chunk->GetPosition().z })) != world->GetChunkMap()->end())
		world->GetChunkMap()->at(WorldManager::GetChunkKey({ chunk->GetPosition().x, chunk->GetPosition().y - 1, chunk->GetPosition().z }))->RefreshBorders();
	if (world->GetChunkMap()->find(WorldManager::GetChunkKey({ chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z + 1})) != world->GetChunkMap()->end())
		world->GetChunkMap()->at(WorldManager::GetChunkKey({ chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z + 1}))->RefreshBorders();
	if (world->GetChunkMap()->find(WorldManager::GetChunkKey({ chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z - 1})) != world->GetChunkMap()->end())
		world->GetChunkMap()->at(WorldManager::GetChunkKey({ chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z - 1}))->RefreshBorders();
}
