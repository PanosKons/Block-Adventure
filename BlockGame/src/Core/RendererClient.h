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
	static void RenderEntities();
	static void RenderWorld(World* world);
	static void RenderUI(double TimeStep);
private:
	static void RenderChunk(Chunk* chunk);
	inline static std::unordered_map<int64_t, ChunkRenderData> ChunkData;
	inline static RenderData UIRenderData;
	inline static RenderData EntityRenderData;
};