#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"
#include "Common/World/Chunk.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBuffer.h"
struct RenderData
{
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBufferTransparent;
	std::unique_ptr<VertexBuffer> m_VertexBufferTransparent;
};
class ChunkRenderer
{
public:
	void RegisterChunk(Vector3<int> ChunkPosition);
	void DrawChunk(Chunk* chunk);
private:
	std::unordered_map<uint64_t, RenderData> ChunkData;
};

