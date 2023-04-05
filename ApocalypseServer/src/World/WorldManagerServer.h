#pragma once
#include "pch.h"
#include "Common/World/WorldManager.h"
class WorldManagerServer : public WorldManager
{
public:
	static void SendAppropriateChunks();
	static void RemoveUnusedChunks();
	static void IncrementRenderDistance(int value = 1);
private:
	static bool ShouldRemoveChunk(Chunk* chunk);

	static inline int RenderDistance = 2;
	static inline int UnloadRenderDistance = RenderDistance + 1;
	static inline std::unordered_map<uint64_t, std::vector<Vector3<int>>> PlayerLoadedChunks;
};

