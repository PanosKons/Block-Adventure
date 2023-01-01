#include "WorldManagerClient.h"
#include "Entities/PlayerClient.h"
#include "Entities/EntityManagerClient.h"
#include "Common/Networking/Packet.h"
#include "Networking/NetworkingClient.h"
#include "Logger.h"
#include "Common/Math/EngineMath.h"
void WorldManagerClient::DeleteOldChunks()
{
	static int RenderDistance = 3;
	Vector3<int> ChunkPosition = Vector::IntVector(EntityManagerClient::GetPlayer().Position) / ChunkSize;

	for (auto [key, chunk] : *WorldManager::BaseWorld->GetChunkMap())
	{
		if (Math::Abs(chunk->GetPosition().x - ChunkPosition.x) >= 3 || Math::Abs(chunk->GetPosition().y - ChunkPosition.y) >= 3 || Math::Abs(chunk->GetPosition().z - ChunkPosition.z) >= 3)
		{
			WorldManager::BaseWorld->DestroyChunk(chunk->GetPosition());
		}
	}
}
void WorldManagerClient::RequestNewChunks()
{
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
