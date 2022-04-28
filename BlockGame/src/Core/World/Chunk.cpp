#include <Engine.h>
#include "Blocks.h"
#include "Chunk.h"
#include "World.h"
#include "Math/Noise.h"
#include "SavingData.h"
#include "GlobalVariables.h"
void Chunk::SpawnStructure(Vector3<int> Positionn, Structure str)
{
	Positionn -= str.Center;
	if (Positionn.x < 0 || Positionn.y < 0 || Positionn.z < 0) return;
	auto a = str.data;
	for (int x = 0; x < StructureSize; x++)
	{
		for (int y = 0; y < StructureSize; y++)
		{
			for (int z = 0; z < StructureSize; z++)
			{
				if ((*a)[x + y * StructureSize + z * StructureSize * StructureSize] == BLOCK_ID::Air) continue;
				if (Positionn.x + x < ChunkSize && Positionn.y + y < ChunkHeight && Positionn.z + z < ChunkSize) {
					delete (*blocks)[Positionn.x + x][Positionn.y + y][Positionn.z + z];
					(*blocks)[Positionn.x + x][Positionn.y + y][Positionn.z + z] = world->MakeBlock((*a)[x + y * StructureSize + z * StructureSize * StructureSize]);
				}
			}
		}
	}
}
static Structure str({ 1,0,1 }, "tree");
Chunk::Chunk(Vector2<int> Position, World* world)
	:Position(Position), world(world)
{
	m_VertexBuffer = std::make_unique<VertexBuffer>();
	m_IndexBuffer = std::make_unique<IndexBuffer>();
	m_VertexBufferTransparent = std::make_unique<VertexBuffer>();
	m_IndexBufferTransparent = std::make_unique<IndexBuffer>();
	auto a = SavingData::LoadChunk(Position);
	if (a != nullptr)
	{
		blocks = a;
		return;
	}
	blocks = new std::array<std::array<std::array<Block*, ChunkSize>, ChunkHeight>, ChunkSize>();
	srand(1);
	std::array<int, ChunkSize* ChunkSize> HeightMap;
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int z = 0; z < ChunkSize; z++)
		{
			HeightMap[x + z * ChunkSize] = Noise::GetYLevel(x + Position.x * 16, z + Position.y * 16);
		}
	}
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkHeight; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				int level = HeightMap[x + z * ChunkSize];
				if (y == level)
				{
					(*blocks)[x][y][z] = new BlockGrass();
				}
				else if (y + 1 == level)
				{
					(*blocks)[x][y][z] = new BlockDirt();
				}
				else if (y + 2 == level)
				{
					(*blocks)[x][y][z] = new BlockDirt();
				}
				else if (y < level)
				{
					if(rand() % 50 == 0)
						(*blocks)[x][y][z] = new BlockIron();
					else
						(*blocks)[x][y][z] = new BlockCobblestone();
				}
				else
				{
					(*blocks)[x][y][z] = new BlockAir();
				}
			}
		}
	}
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int z = 0; z < ChunkSize; z++)
		{
			auto y = HeightMap[x + z * ChunkSize];
			if (rand() % 100 == 0 && y > 25)
			{
				SpawnStructure({ x,y + 1 ,z }, str);
			}
		}
	}
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkHeight; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				(*blocks)[x][y][z]->Position = { x + ChunkSize * Position.x,y,z + ChunkSize * Position.y };
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
	return (*blocks)[Position.x][Position.y][Position.z];
}
#define ONEOVER16 0.0625f
void Chunk::DrawBlock(Block* block)
{
	VertexBuffer* vertexBuffer;
	block->Transparent ? vertexBuffer = m_VertexBufferTransparent.get() : vertexBuffer = m_VertexBuffer.get();
	IndexBuffer* indexBuffer;
	block->Transparent ? indexBuffer = m_IndexBufferTransparent.get() : indexBuffer = m_IndexBuffer.get();
	if (!(block->RenderedSides & (unsigned char)64)) return;
	std::array<unsigned char, 6>& arr = blockProperties[block->GetBlockId()].textureSides;
	Vertex a;
	a.texId = 0.0f;
	if (block->RenderedSides & (unsigned char)1) {
		float texcordsX = ((arr[0]) % 16) / 16.0f;
		float texcordsY = ((arr[0]) / 16) / 16.0f;
		a.color = { 0.9f,0.9f,0.9f,1.0f };
		a.texCords = { texcordsX, texcordsY };
		a.position = Vector::FloatVector(block->Position);
		vertexBuffer->Add(a);
		a.position.x += 1.0f;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.y += 1.0f;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.x = (float)block->Position.x;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
	}
	if (block->RenderedSides & (unsigned char)2) {
		float texcordsX = ((arr[1]) % 16) / 16.0f;
		float texcordsY = ((arr[1]) / 16) / 16.0f;
		a.color = { 0.85f,0.85f,0.85f,1.0f };
		a.position = Vector::FloatVector(block->Position);
		a.texCords = { texcordsX, texcordsY };
		a.position.x += 1.0f;
		vertexBuffer->Add(a);
		a.position.z += 1.0f;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.y += 1.0f;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.z = (float)block->Position.z;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
	}
	if (block->RenderedSides & (unsigned char)4) {
		float texcordsX = ((arr[2]) % 16) / 16.0f;
		float texcordsY = ((arr[2]) / 16) / 16.0f;
		a.color = { 0.75f,0.75f,0.75f,1.0f };
		a.position = Vector::FloatVector(block->Position);
		a.texCords = { texcordsX, texcordsY };
		a.position.z += 1.0f;
		a.position.x += 1.0f;
		vertexBuffer->Add(a);
		a.position.x = (float)block->Position.x;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.y += 1.0f;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.x += 1.0f;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
	}
	if (block->RenderedSides & (unsigned char)8) {
		float texcordsX = ((arr[3]) % 16) / 16.0f;
		float texcordsY = ((arr[3]) / 16) / 16.0f;
		a.color = { 0.8f,0.8f,0.8f,1.0f };
		a.position = Vector::FloatVector(block->Position);
		a.position.z += 1.0f;
		a.texCords = { texcordsX, texcordsY };
		vertexBuffer->Add(a);
		a.position.z = (float)block->Position.z;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.y += 1.0f;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.z += 1.0f;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
	}
	if (block->RenderedSides & (unsigned char)16) {
		float texcordsX = ((arr[4]) % 16) / 16.0f;
		float texcordsY = ((arr[4]) / 16) / 16.0f;
		a.color = { 0.7f,0.7f,0.7f,1.0f };
		a.position = Vector::FloatVector(block->Position);
		a.texCords = { texcordsX, texcordsY };
		vertexBuffer->Add(a);
		a.position.z += 1.0f;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.x += 1.0f;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.z = (float)block->Position.z;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
	}
	if (block->RenderedSides & (unsigned char)32) {
		float texcordsX = ((arr[5]) % 16) / 16.0f;
		float texcordsY = ((arr[5]) / 16) / 16.0f;
		a.color = { 1.0f,1.0f,1.0f,1.0f };
		a.position = Vector::FloatVector(block->Position);
		a.position.y += 1.0f;
		a.texCords = { texcordsX, texcordsY };
		vertexBuffer->Add(a);
		a.position.x += 1.0f;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.z += 1.0f;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.x = (float)block->Position.x;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
	}
	indexBuffer->AddCuboid(block->RenderedSides);
}
void Chunk::Draw() {
	m_VertexBuffer->Clear();
	m_IndexBuffer->Clear();
	m_VertexBufferTransparent->Clear();
	m_IndexBufferTransparent->Clear();
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
	m_VertexBuffer->Bind();
	m_VertexBuffer->Allocate();
	m_VertexBufferTransparent->Bind();
	m_VertexBufferTransparent->Allocate();
}
void Chunk::UpdateAllBlocks()
{
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkHeight; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				(*blocks)[x][y][z]->Update();
			}
		}
	}
}