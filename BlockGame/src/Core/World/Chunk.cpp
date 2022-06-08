#include <Engine.h>
#include "Blocks.h"
#include "Chunk.h"
#include "World.h"
#include "Math/Noise.h"
#include "SavingData.h"
#include "GlobalVariables.h"
#include "Networking.h"
void Chunk::SpawnStructure(Vector3<int> RelativePosition, std::string&& name)
{
	static Structure* structure = SavingData::LoadStructure(name.c_str());
	for (int x = 0; x < StructureSize; x++)
	{
		for (int y = 0; y < StructureSize; y++)
		{
			for (int z = 0; z < StructureSize; z++)
			{
				if (structure->data[x + y * StructureSize + z * StructureSize * StructureSize] == BLOCK_ID::Air) continue;
				if (RelativePosition.x + x < ChunkSize && RelativePosition.y + y < ChunkSize && RelativePosition.z + z < ChunkSize)
				{
					(*blocks)[RelativePosition.x + x][RelativePosition.y + y][RelativePosition.z + z].blockId = (unsigned short)structure->data[x + y * StructureSize + z * StructureSize * StructureSize];
				}
			}
		}
	}
}
Chunk::Chunk(Vector3<int> Position, std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks, World* world)
	:Position(Position), world(world), blocks(blocks)
{
	m_VertexBuffer = std::make_unique<VertexBuffer>();
	m_IndexBuffer = std::make_unique<IndexBuffer>();
	m_VertexBufferTransparent = std::make_unique<VertexBuffer>();
	m_IndexBufferTransparent = std::make_unique<IndexBuffer>();

	/*SavingData::LoadChunk(Position, &blocks);
	if (blocks != nullptr)
		return;

	for (int x = 0; x < ChunkSize; x++)
	{
		for (int z = 0; z < ChunkSize; z++)
		{
			int y = HeightMap[x + z * ChunkSize] + 1;

			if (rand() % 100 == 0 && y > 40 && y / ChunkSize == Position.y)
			{
				SpawnStructure({ x,y % ChunkSize ,z }, "tree");
			}
		}
	}*/
}
Chunk::~Chunk()
{
	SavingData::SaveChunk(Position,blocks);
	delete blocks;
}
void UpdateAllBlocksAsync(std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks,Chunk* chunk)
{
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				chunk->GetBlock({ x,y,z }).Update();
			}
		}
	}
}
void UpdateBorderBlocksAsync(std::array<std::array<std::array<BlockData, ChunkSize>, ChunkSize>, ChunkSize>* blocks, Chunk* chunk)
{
	for (int x = 0; x < ChunkSize; x += ChunkSize - 1)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				chunk->GetBlock({ x,y,z }).Update();
			}
		}
	}
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 1; z < ChunkSize; z += ChunkSize - 2)
			{
				chunk->GetBlock({ x,y,z }).Update();
			}
		}
	}
	for (int x = 1; x < ChunkSize - 1; x++)
	{
		for (int y = 0; y < ChunkSize; y += ChunkSize - 1)
		{
			for (int z = 1; z < ChunkSize - 1; z++)
			{
				chunk->GetBlock({ x,y,z }).Update();
			}
		}
	}
}
void Chunk::UpdateAllBlocks()
{
	std::thread worker(UpdateAllBlocksAsync,blocks,this);
	worker.detach();
}
void Chunk::UpdateBorderBlocks()
{
	std::thread worker(UpdateBorderBlocksAsync, blocks,this);
	worker.detach();
}

Block Chunk::GetBlock(Vector3<int> RelativePosition) const
{
	return World::MakeBlock(&(*blocks)[RelativePosition.x][RelativePosition.y][RelativePosition.z], { RelativePosition.x + Position.x * ChunkSize,RelativePosition.y + Position.y * ChunkSize, RelativePosition.z + Position.z * ChunkSize });
}
Vector3<int> Chunk::GetPosition() const
{
	return Position;
}

VertexBuffer* Chunk::GetVertexBuffer() const
{
	return m_VertexBuffer.get();
}
VertexBuffer* Chunk::GetVertexBufferTransparent() const
{
	return m_VertexBufferTransparent.get();
}
IndexBuffer* Chunk::GetIndexBuffer() const
{
	return m_IndexBuffer.get();
}
IndexBuffer* Chunk::GetIndexBufferTransparent() const
{
	return m_IndexBufferTransparent.get();
}
#define ONEOVER16 0.0625f
void Chunk::DrawBlock(Block block)
{
	VertexBuffer* vertexBuffer;
	block.GetTransparent() ? vertexBuffer = m_VertexBufferTransparent.get() : vertexBuffer = m_VertexBuffer.get();
	IndexBuffer* indexBuffer;
	block.GetTransparent() ? indexBuffer = m_IndexBufferTransparent.get() : indexBuffer = m_IndexBuffer.get();
	std::array<unsigned char, 6> arr = block.GetBlockProperties().textureSides;
	if (arr[0] == INVALID) return;
	Vertex a;
	a.texId = 0.0f;
	float alpha = 1.0f;
	if (block.GetBlockId() == BLOCK_ID::Water) alpha = 0.4f;
	if (block.data->RenderedSides & (unsigned char)1) {
		float texcordsX = ((arr[0]) % 16) / 16.0f;
		float texcordsY = ((arr[0]) / 16) / 16.0f;
		a.color = { 0.9f,0.9f,0.9f,alpha };
		a.texCords = { texcordsX, texcordsY };
		a.position = Vector::FloatVector(block.Position);
		vertexBuffer->Add(a);
		a.position.x += 1.0f;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.y += 1.0f;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.x = (float)block.Position.x;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
	}
	if (block.data->RenderedSides & (unsigned char)2) {
		float texcordsX = ((arr[1]) % 16) / 16.0f;
		float texcordsY = ((arr[1]) / 16) / 16.0f;
		a.color = { 0.85f,0.85f,0.85f,alpha };
		a.position = Vector::FloatVector(block.Position);
		a.texCords = { texcordsX, texcordsY };
		a.position.x += 1.0f;
		vertexBuffer->Add(a);
		a.position.z += 1.0f;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.y += 1.0f;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.z = (float)block.Position.z;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
	}
	if (block.data->RenderedSides & (unsigned char)4) {
		float texcordsX = ((arr[2]) % 16) / 16.0f;
		float texcordsY = ((arr[2]) / 16) / 16.0f;
		a.color = { 0.75f,0.75f,0.75f,alpha };
		a.position = Vector::FloatVector(block.Position);
		a.texCords = { texcordsX, texcordsY };
		a.position.z += 1.0f;
		a.position.x += 1.0f;
		vertexBuffer->Add(a);
		a.position.x = (float)block.Position.x;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.y += 1.0f;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.x += 1.0f;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
	}
	if (block.data->RenderedSides & (unsigned char)8) {
		float texcordsX = ((arr[3]) % 16) / 16.0f;
		float texcordsY = ((arr[3]) / 16) / 16.0f;
		a.color = { 0.8f,0.8f,0.8f,alpha };
		a.position = Vector::FloatVector(block.Position);
		a.position.z += 1.0f;
		a.texCords = { texcordsX, texcordsY };
		vertexBuffer->Add(a);
		a.position.z = (float)block.Position.z;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.y += 1.0f;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.z += 1.0f;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
	}
	if (block.data->RenderedSides & (unsigned char)16) {
		float texcordsX = ((arr[4]) % 16) / 16.0f;
		float texcordsY = ((arr[4]) / 16) / 16.0f;
		a.color = { 0.7f,0.7f,0.7f,alpha };
		a.position = Vector::FloatVector(block.Position);
		a.texCords = { texcordsX, texcordsY };
		vertexBuffer->Add(a);
		a.position.z += 1.0f;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.x += 1.0f;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.z = (float)block.Position.z;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
	}
	if (block.data->RenderedSides & (unsigned char)32) {
		float texcordsX = ((arr[5]) % 16) / 16.0f;
		float texcordsY = ((arr[5]) / 16) / 16.0f;
		a.color = { 1.0f,1.0f,1.0f,alpha };
		a.position = Vector::FloatVector(block.Position);
		a.position.y += 1.0f;
		a.texCords = { texcordsX, texcordsY };
		vertexBuffer->Add(a);
		a.position.x += 1.0f;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.z += 1.0f;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.x = (float)block.Position.x;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
	}
	indexBuffer->AddCuboid(block.data->RenderedSides);
}
void Chunk::Draw() {
	m_VertexBuffer->Clear();
	m_IndexBuffer->Clear();
	m_VertexBufferTransparent->Clear();
	m_IndexBufferTransparent->Clear();
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				DrawBlock(GetBlock({x,y,z}));
			}
		}
	}
	m_VertexBuffer->Bind();
	m_VertexBuffer->Allocate();
	m_VertexBufferTransparent->Bind();
	m_VertexBufferTransparent->Allocate();
}