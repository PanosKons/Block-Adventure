#pragma once
#include "../Math/Vector.h"
#include <array>
#include <memory>
#include "../Rendering/VertexBuffer.h"
#include "../Rendering/IndexBuffer.h"
#include "Block.h"
class World;
constexpr int ChunkSize = 16;
constexpr int ChunkHeight = 128;
class Chunk {
public:
	bool Changed = true;
	void UpdateAllBlocks();
	~Chunk();
	Chunk(Vector2<int> Position, World* world);
	World* world;
	Vector2<int> Position;
	Block* GetBlock(Vector3<int> Position) const;
	void Draw();
	std::array<std::array<std::array<Block, ChunkSize>, ChunkHeight>, ChunkSize>* blocks;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
private:
	void DrawBlock(Block& block);
};