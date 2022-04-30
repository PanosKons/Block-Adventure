#pragma once
#include <Engine.h>
#include "Math/Vector.h"
#include "Structure.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "../Blocks.h"
class World;
constexpr int ChunkSize = 32;
constexpr int ChunkHeight = 128;
class Chunk {
public:
	Chunk(Vector2<int> ChunkPosition, World* world);
	~Chunk();
	void UpdateAllBlocks();
	void UpdateBorderBlocks();
	Block* GetBlock(Vector3<int> RelativePosition) const;
	void SpawnStructure(Vector3<int> RelativePosition, std::string&& name, bool GenerationStage = true);
	void Draw();
	bool Changed = true;
	VertexBuffer* GetVertexBuffer() const;
	VertexBuffer* GetVertexBufferTransparent() const;
	IndexBuffer* GetIndexBuffer() const;
	IndexBuffer* GetIndexBufferTransparent() const;
	Vector2<int> GetPosition() const;
	std::array<std::array<std::array<Block*, ChunkSize>, ChunkHeight>, ChunkSize>* GetBlocks() const;
private:
	World* world;
	Vector2<int> Position;
	std::array<std::array<std::array<Block*, ChunkSize>, ChunkHeight>, ChunkSize>* blocks;

	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBufferTransparent;
	std::unique_ptr<VertexBuffer> m_VertexBufferTransparent;

	void DrawBlock(Block* block);
};