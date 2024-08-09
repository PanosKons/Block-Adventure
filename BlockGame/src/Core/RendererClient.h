#pragma once
#include "pch.h"
#include "Common/World/World.h"
#include "Rendering/RenderData.h"

constexpr float SlotsX = 64.0f;

struct ChunkRenderData
{
	RenderData Solid;
	RenderData Transparent;
	bool Populated = false;
};
class RendererClient
{
public:
	static void RenderBlock(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, Block& block);
	static void RenderEntities();
	static void RenderWorld(World* world);
	static void RenderUI(double TimeStep);

	static Vector2<float> SlotToPixel(Vector2<float> cords);
	
private:
	static void RenderChunk(Chunk* chunk);
	inline static std::unordered_map<int64_t, ChunkRenderData> ChunkData;
	inline static RenderData UIRenderData;
	inline static RenderData EntityRenderData;
};