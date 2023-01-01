#include "pch.h"
#include "WorldManagerServer.h"
#include "EntityManagerServer.h"
#include "NetworkingServer.h"
#include "Common/Math/EngineMath.h"

void WorldManagerServer::SendAppropriateChunks()
{
	for (auto& [UUID, player] : EntityManagerServer::Players)
	{
		if (player == nullptr) continue; //Shouldnt be here (multithreading issue)
		Vector3<int> ChunkPosition = Vector::IntVector(player->Position) / ChunkSize;
		auto& PlayerChunks = PlayerLoadedChunks[UUID];
		for (int x = ChunkPosition.x - RenderDistance; x <= ChunkPosition.x + RenderDistance; x++)
		{
			for (int y = ChunkPosition.y - RenderDistance; y <= ChunkPosition.y + RenderDistance; y++)
			{
				for (int z = ChunkPosition.z - RenderDistance; z <= ChunkPosition.z + RenderDistance; z++)
				{
					Vector3<int> NewChunkPosition = { x,y,z };
					if (std::find(PlayerChunks.begin(), PlayerChunks.end(), NewChunkPosition) == PlayerChunks.end())
					{
						if (WorldManager::BaseWorld->GetChunkMap()->find(WorldManager::GetChunkKey(NewChunkPosition)) == WorldManager::BaseWorld->GetChunkMap()->end())
						{
							WorldManager::BaseWorld->CreateChunk(NewChunkPosition, WorldManager::GenerateChunk(NewChunkPosition));
							WorldManager::BaseWorld->GetChunkDirect(NewChunkPosition)->Refresh();
						}
						{
							Packet<SendNewChunk> packet;
							packet.InitMemory();
							packet.AddPacketData(PACKET_ID::NewChunk);
							packet.AddPacketData(NewChunkPosition);
							NetworkingServer::SendPacketToClient(player->credentials, packet);
						}
						{
							Packet<ChunkPacketSize> SendPacket;
							SendPacket.SetPacket((std::array<char, ChunkPacketSize>*)WorldManager::BaseWorld->GetChunkDirect(NewChunkPosition)->GetBlocks());
							NetworkingServer::SendPacketToClient(player->credentials, SendPacket);
							SendPacket.SetPacket(nullptr);
						}
						PlayerChunks.push_back(NewChunkPosition);
					}
				}
			}
		}
		for (Vector3<int>& ExixstingChunkPosition : PlayerChunks)
		{
			if (Math::Abs(ExixstingChunkPosition.x - ChunkPosition.x) > UnloadRenderDistance || Math::Abs(ExixstingChunkPosition.y - ChunkPosition.y) > UnloadRenderDistance || Math::Abs(ExixstingChunkPosition.z - ChunkPosition.z) > UnloadRenderDistance)
			{
				Packet<SendDeleteChunk> packet;
				packet.InitMemory();
				packet.AddPacketData(PACKET_ID::DeleteChunk);
				packet.AddPacketData(ExixstingChunkPosition);
				NetworkingServer::SendPacketToClient(player->credentials, packet);
				PlayerChunks.erase(std::find(PlayerChunks.begin(), PlayerChunks.end(), ExixstingChunkPosition));
			}
		}
	}
}
bool WorldManagerServer::ShouldRemoveChunk(Chunk* chunk)
{
	Vector3<int> ChunkToRemovePosition = chunk->GetPosition();
	for (auto& [UUID, player] : EntityManagerServer::Players)
	{
		Vector3<int> ChunkPosition = Vector::IntVector(player->Position) / ChunkSize;
		if (Math::Abs(ChunkToRemovePosition.x - ChunkPosition.x) <= UnloadRenderDistance && Math::Abs(ChunkToRemovePosition.y - ChunkPosition.y) <= UnloadRenderDistance && Math::Abs(ChunkToRemovePosition.z - ChunkPosition.z) <= UnloadRenderDistance)
		{
			return false;
		}
	}
	return true;
}
void WorldManagerServer::RemoveUnusedChunks()
{
	static int tick = 0;
	if (tick >= 1000)
	{
		tick = 0;

		auto ChunkMapCopy = *WorldManager::BaseWorld->GetChunkMap();
		for (auto&[chunkKey, chunk] : ChunkMapCopy)
		{
			if (ShouldRemoveChunk(chunk) == true)
			{
				WorldManager::BaseWorld->DestroyChunk(chunk->GetPosition());
			}
		}
	}
	tick++;
}
