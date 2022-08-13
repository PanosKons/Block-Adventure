#pragma once
#include "pch.h"
#include "Common/World/World.h"
#include "Rendering/RenderData.h"

struct ChunkRenderData
{
	RenderData Solid;
	RenderData Transparent;
	bool Populated = false;
};
class RendererClient
{
public:
	static void RenderWorld(World* world);
	static void RenderEntities();
	static void RenderUI();
	static void UpdateChunk(World* world, Chunk* chunk);
private:
	inline static std::unordered_map<int64_t, ChunkRenderData> ChunkData;
};