#include "WorldManagerClient.h"
#include "Entities/PlayerClient.h"
#include "Entities/EntityManagerClient.h"
#include "Common/Networking/Packet.h"
#include "Networking/Networking.h"

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
