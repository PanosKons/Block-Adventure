#include "Chunk.h"
#include "World.h"
Chunk::Chunk(Vector2<int> Position, World* world)
	:Position(Position), world(world)
{
	m_VertexBuffer = std::make_unique<VertexBuffer>();
	m_VertexBuffer->Bind();
	m_VertexBuffer->Allocate();
	m_IndexBuffer = std::make_unique<IndexBuffer>();
	blocks = new std::array<std::array<std::array<Block, ChunkSize>, ChunkHeight>, ChunkSize>();
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkHeight; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				(*blocks)[x][y][z].Transform = { x + ChunkSize * Position.x,y,z + ChunkSize * Position.y };
				double level = (sin(6 * (Position.x * ChunkSize + x)) * cos(6 * (Position.y * ChunkSize + z)) * 7 + 47);
				if (y == (int)level)
				{
					(*blocks)[x][y][z].id = Grass_block;
				}
				else if (y < level)
				{
					(*blocks)[x][y][z].id = Cobblestone;
				}
				if (y == (int)(level)+1 && (y + x + z) % 15 == 0)
				{
					(*blocks)[x][y][z].id = Log;
					(*blocks)[x][y + 1][z].id = Log;
					(*blocks)[x][y + 2][z].id = Log;
					(*blocks)[x][y + 3][z].id = Log;
				}
			}
		}
	}
}
Chunk::~Chunk()
{
	delete blocks;
}
Block* Chunk::GetBlock(Vector3<int> Position) const
{
	return &(*blocks)[Position.x][Position.y][Position.z];
}
void Chunk::DrawBlock(Block& block)
{
	if (block.id == BLOCK_ID::Air) return;
	Vertex a;
	a.texId = (float)(block.id - 1);
	if (block.RenderedSides & (unsigned char)1) {
		a.color = { 0.9f,0.9f,0.9f,1.0f };
		a.position = Vector::FloatVector(block.Transform);
		m_VertexBuffer->Add(a);
		a.position.x += 1.0f;
		a.texCords.x = 1;
		m_VertexBuffer->Add(a);
		a.position.y += 1.0f;
		a.texCords.y = 1;
		m_VertexBuffer->Add(a);
		a.position.x = block.Transform.x;
		a.texCords.x = 0;
		m_VertexBuffer->Add(a);
	}
	if (block.RenderedSides & (unsigned char)2) {
		a.color = { 0.85f,0.85f,0.85f,1.0f };
		a.position = Vector::FloatVector(block.Transform);
		a.texCords.y = 0;
		a.position.x += 1.0f;
		m_VertexBuffer->Add(a);
		a.position.z += 1.0f;
		a.texCords.x = 1;
		m_VertexBuffer->Add(a);
		a.position.y += 1.0f;
		a.texCords.y = 1;
		m_VertexBuffer->Add(a);
		a.position.z = block.Transform.z;
		a.texCords.x = 0;
		m_VertexBuffer->Add(a);
	}
	if (block.RenderedSides & (unsigned char)4) {
		a.color = { 0.75f,0.75f,0.75f,1.0f };
		a.position = Vector::FloatVector(block.Transform);
		a.texCords.y = 0;
		a.position.z += 1.0f;
		a.position.x += 1.0f;
		m_VertexBuffer->Add(a);
		a.position.x = block.Transform.x;
		a.texCords.x = 1;
		m_VertexBuffer->Add(a);
		a.position.y += 1.0f;
		a.texCords.y = 1;
		m_VertexBuffer->Add(a);
		a.position.x += 1.0f;
		a.texCords.x = 0;
		m_VertexBuffer->Add(a);
	}
	if (block.RenderedSides & (unsigned char)8) {
		a.color = { 0.8f,0.8f,0.8f,1.0f };
		a.position = Vector::FloatVector(block.Transform);
		a.position.z += 1.0f;
		a.texCords.y = 0;
		m_VertexBuffer->Add(a);
		a.position.z = block.Transform.z;
		a.texCords.x = 1;
		m_VertexBuffer->Add(a);
		a.position.y += 1.0f;
		a.texCords.y = 1;
		m_VertexBuffer->Add(a);
		a.position.z += 1.0f;
		a.texCords.x = 0;
		m_VertexBuffer->Add(a);
	}
	if (block.RenderedSides & (unsigned char)16) {
		a.color = { 0.7f,0.7f,0.7f,1.0f };
		a.position = Vector::FloatVector(block.Transform);
		a.texCords.y = 0;
		m_VertexBuffer->Add(a);
		a.position.z += 1.0f;
		a.texCords.x = 1;
		m_VertexBuffer->Add(a);
		a.position.x += 1.0f;
		a.texCords.y = 1;
		m_VertexBuffer->Add(a);
		a.position.z = block.Transform.z;
		a.texCords.x = 0;
		m_VertexBuffer->Add(a);
	}
	if (block.RenderedSides & (unsigned char)32) {
		a.color = { 1.0f,1.0f,1.0f,1.0f };
		a.position = Vector::FloatVector(block.Transform);
		a.position.y += 1.0f;
		a.texCords.y = 0;
		m_VertexBuffer->Add(a);
		a.position.x += 1.0f;
		a.texCords.x = 1;
		m_VertexBuffer->Add(a);
		a.position.z += 1.0f;
		a.texCords.y = 1;
		m_VertexBuffer->Add(a);
		a.position.x = block.Transform.x;
		a.texCords.x = 0;
		m_VertexBuffer->Add(a);
	}
	m_IndexBuffer->AddCuboid(block.RenderedSides);
}
void Chunk::Draw() {
	m_VertexBuffer->Bind();
	m_VertexBuffer->Clear();
	m_IndexBuffer->Clear();
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkHeight; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				DrawBlock((*blocks)[x][y][z]);
			}
		}
	}
	m_VertexBuffer->Allocate();
}
void Chunk::UpdateAllBlocks()
{
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkHeight; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				(*blocks)[x][y][z].Update();
			}
		}
	}
}