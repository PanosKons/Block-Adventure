#pragma once
#include "pch.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBuffer.h"
#include "Common/Math/Vector.h"
#include "Common/Blocks/Block.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Common/World/Chunk.h"
constexpr float charWidthOffset = 28.0f;
constexpr float charHeightOffset = 32.0f;
constexpr float charWidth = 32.0f;
constexpr float charHeight = 32.0f;
namespace Renderer
{
	int CreateWindow(const std::string& name);
	void DrawChunk(Chunk* chunk);
	void DrawChunkTransparent(Chunk* chunk);
	void DrawGeometry(VertexBuffer& vb, IndexBuffer& ib);
	void DrawGeometry(VertexBuffer& vb, IndexBuffer& ib, unsigned int count, unsigned int offset);
	void DrawText(VertexBuffer& vb, IndexBuffer& ib,std::string Text,Vector3<float> position);
	void DrawSquare(VertexBuffer& vb, IndexBuffer& ib, Vector3<float> Position, Vector2<float> Size, Vector4<float> Color, Vector2<float> TexCords, Vector2<float> TexSize, float TextureID);
	void Run();
	void SetBackroundColorAndClear(std::array<float,4> color);
	void SetUIView();
	void SetPlayerView();
};
