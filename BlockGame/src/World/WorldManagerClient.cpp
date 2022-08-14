#include "WorldManagerClient.h"
#include "Entities/PlayerClient.h"
#include "Entities/EntityManagerClient.h"
#include "Common/Networking/Packet.h"
#include "Networking/Networking.h"

void WorldManagerClient::RequestNewChunks()
{
	if (WorldManager::BaseWorld->GetChunk(Vector::IntVector(EntityManagerClient::GetPlayer().Position)) == nullptr)
	{
		Vector3<int> ChunkPosition = Vector::IntVector(EntityManagerClient::GetPlayer().Position) / ChunkSize;
		Packet<DefaultPacketSize> packet;
		packet.InitMemory();
		packet.AddPacketData(PACKET_ID::RequestChunk);
		packet.AddPacketData(ChunkPosition);
		Networking::SendPacketToServer(packet);
	}
}
