#pragma once
#include <string>
#include <array>
#include <list>
#include "../Math/Vector.h"
#include "../Blocks/Block.h"
#include <memory>
#include <vector>
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "Chunk.h"
namespace Renderer
{
	int CreateWindow(const std::string& name);
	void DrawChunk(Chunk* chunk);
	void DrawGeometry(VertexBuffer& vb, IndexBuffer& ib);
	void DrawGeometry(VertexBuffer& vb, IndexBuffer& ib, unsigned int count, unsigned int offset);
	void Run();
};
