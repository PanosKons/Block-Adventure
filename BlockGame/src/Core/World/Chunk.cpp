#include <Engine.h>
#include "Blocks.h"
#include "Chunk.h"
#include "World.h"
#include "Math/Noise.h"
#include "SavingData.h"
#include "GlobalVariables.h"
void Chunk::SpawnStructure(Vector3<int> RelativePosition, std::string&& name, bool GenerationStage)
{
	static Structure* structure = SavingData::LoadStructure(name.c_str());
	if(GenerationStage == true)
		for (int x = 0; x < StructureSize; x++)
		{
			for (int y = 0; y < StructureSize; y++)
			{
				for (int z = 0; z < StructureSize; z++)
				{
					if (structure->data[x + y * StructureSize + z * StructureSize * StructureSize] == BLOCK_ID::Air) continue;
						if (RelativePosition.x + x < ChunkSize && RelativePosition.y + y < ChunkSize && RelativePosition.z + z < ChunkSize)
					{
					delete (*blocks)[RelativePosition.x + x][RelativePosition.y + y][RelativePosition.z + z];
					(*blocks)[RelativePosition.x + x][RelativePosition.y + y][RelativePosition.z + z] = world->MakeBlock(structure->data[x + y * StructureSize + z * StructureSize * StructureSize]);
					}
				}
			}
	}
	else
a		for (int x = 0; x < StructureSize; x++)
		{
			for (int y = 0; y < StructureSize; y++)
			{
				for (int z = 0; z < StructureSize; z++)
				{
					if(structure->data[x + y * StructureSize + z * StructureSize * StructureSize] == BLOCK_ID::Air) continue;
					world->GetBlock({ RelativePosition.x + ChunkSize * Position.x + x,RelativePosition.y + Position.y* ChunkSize + y,RelativePosition.z + z + ChunkSize * Position.z })->OnBreak(structure->data[x + y * StructureSize + z * StructureSize * StructureSize]);
				}
			}
		}
}
Chunk::Chunk(Vector3<int> Position, World* world)
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
	blocks = new std::array<std::array<std::array<Block*, ChunkSize>, ChunkSize>, ChunkSize>();
	srand(1);
	std::array<int, ChunkSize* ChunkSize> HeightMap;
	std::array<int, ChunkSize* ChunkSize> BiomeMap;
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int z = 0; z < ChunkSize; z++)
		{
			HeightMap[x + z * ChunkSize] = Noise::GetYLevel(x + Position.x * ChunkSize, z + Position.z * ChunkSize);
			BiomeMap[x + z * ChunkSize] = Noise::GetBiomeTemperature(x + Position.x * ChunkSize, z + Position.z * ChunkSize);
		}
	}
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				int ylevel = y + ChunkSize * Position.y;
				int level = HeightMap[x + z * ChunkSize];
				if (ylevel > level && ylevel <= 30)
				{
					(*blocks)[x][y][z] = new BlockWater();
				}
				else if (ylevel == level)
				{
					if (ylevel < 30)
					{
						(*blocks)[x][y][z] = new BlockDirt();
					}
					else if (BiomeMap[x + z * ChunkSize] == 0)
					{
						(*blocks)[x][y][z] = new BlockCobblestone();
					}
					else if (BiomeMap[x + z * ChunkSize] == 1)
					{
						(*blocks)[x][y][z] = new BlockDirt();
					}
					else if (BiomeMap[x + z * ChunkSize] == 2)
					{
						(*blocks)[x][y][z] = new BlockDryGrass();
					}
					else
					{
						(*blocks)[x][y][z] = new BlockGrass();
					}
				}
				else if (ylevel + 1 == level)
				{
					(*blocks)[x][y][z] = new BlockDirt();
				}
				else if (ylevel + 2 == level)
				{
					(*blocks)[x][y][z] = new BlockDirt();
				}
				else if (ylevel < level)
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
			int y = HeightMap[x + z * ChunkSize] + 1;

			if (rand() % 100 == 0 && y > 40 && y / ChunkSize == Position.y)
			{
				SpawnStructure({ x,y % ChunkSize ,z }, "tree");
			}
		}
	}
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				(*blocks)[x][y][z]->Position = { x + ChunkSize * Position.x,y + ChunkSize * Position.y,z + ChunkSize * Position.z };
			}
		}
	}
}
Chunk::~Chunk()
{
	SavingData::SaveChunk(this);
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				delete (*blocks)[x][y][z];
			}
		}
	}
	delete blocks;
}
void UpdateAllBlocksAsync(std::array<std::array<std::array<Block*, ChunkSize>, ChunkSize>, ChunkSize>* blocks)
{
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				(*blocks)[x][y][z]->Update();
			}
		}
	}
}
void UpdateBorderBlocksAsync(std::array<std::array<std::array<Block*, ChunkSize>, ChunkSize>, ChunkSize>* blocks)
{
	for (int x = 0; x < ChunkSize; x += ChunkSize - 1)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				(*blocks)[x][y][z]->Update();
			}
		}
	}
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 1; z < ChunkSize; z += ChunkSize - 2)
			{
				(*blocks)[x][y][z]->Update();
			}
		}
	}
	for (int x = 1; x < ChunkSize - 1; x++)
	{
		for (int y = 0; y < ChunkSize; y += ChunkSize - 1)
		{
			for (int z = 1; z < ChunkSize - 1; z++)
			{
				(*blocks)[x][y][z]->Update();
			}
		}
	}
}
void Chunk::UpdateAllBlocks()
{
	std::thread worker(UpdateAllBlocksAsync,blocks);
	worker.detach();
}
void Chunk::UpdateBorderBlocks()
{
	std::thread worker(UpdateBorderBlocksAsync, blocks);
	worker.detach();
}

Block* Chunk::GetBlock(Vector3<int> Position) const
{
	return (*blocks)[Position.x][Position.y][Position.z];
}
Vector3<int> Chunk::GetPosition() const
{
	return Position;
}
std::array<std::array<std::array<Block*, ChunkSize>, ChunkSize>, ChunkSize>* Chunk::GetBlocks() const
{
	return blocks;
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
void Chunk::DrawBlock(Block* block)
{
	VertexBuffer* vertexBuffer;
	block->Transparent ? vertexBuffer = m_VertexBufferTransparent.get() : vertexBuffer = m_VertexBuffer.get();
	IndexBuffer* indexBuffer;
	block->Transparent ? indexBuffer = m_IndexBufferTransparent.get() : indexBuffer = m_IndexBuffer.get();
	if (!(block->RenderedSides & (unsigned char)64)) return;
	std::array<unsigned char, 6> arr = block->GetBlockProperties().textureSides;
	Vertex a;
	a.texId = 0.0f;
	float alpha = 1.0f;
	if (block->GetBlockId() == BLOCK_ID::Water) alpha = 0.4f;
	if (block->RenderedSides & (unsigned char)1) {
		float texcordsX = ((arr[0]) % 16) / 16.0f;
		float texcordsY = ((arr[0]) / 16) / 16.0f;
		a.color = { 0.9f,0.9f,0.9f,alpha };
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
		a.color = { 0.85f,0.85f,0.85f,alpha };
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
		a.color = { 0.75f,0.75f,0.75f,alpha };
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
		a.color = { 0.8f,0.8f,0.8f,alpha };
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
		a.color = { 0.7f,0.7f,0.7f,alpha };
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
		a.color = { 1.0f,1.0f,1.0f,alpha };
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
		for (int y = 0; y < ChunkSize; y++)
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