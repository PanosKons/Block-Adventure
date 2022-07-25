#include "ChunkRenderer.h"
#include "Common/World/WorldManager.h"

void ChunkRenderer::RegisterChunk(Vector3<int> ChunkPosition)
{
	ChunkData.emplace(WorldManager::GetChunkKey(ChunkPosition.x, ChunkPosition.y,ChunkPosition.z),RenderData());
	RenderData& renderData = ChunkData[WorldManager::GetChunkKey(ChunkPosition.x, ChunkPosition.y, ChunkPosition.z)];
	renderData.m_VertexBuffer = std::make_unique<VertexBuffer>();
	renderData.m_IndexBuffer = std::make_unique<IndexBuffer>();
	renderData.m_VertexBufferTransparent = std::make_unique<VertexBuffer>();
	renderData.m_IndexBufferTransparent = std::make_unique<IndexBuffer>();
}
#define ONEOVER16 0.0625f
void DrawBlock(Block block,RenderData& renderData)
{
	VertexBuffer* vertexBuffer;
	block.GetTransparent() ? vertexBuffer = renderData.m_VertexBufferTransparent.get() : vertexBuffer = renderData.m_VertexBuffer.get();
	IndexBuffer* indexBuffer;
	block.GetTransparent() ? indexBuffer = renderData.m_IndexBufferTransparent.get() : indexBuffer = renderData.m_IndexBuffer.get();
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
void ChunkRenderer::DrawChunk(Chunk* chunk) {
	RenderData& renderData = ChunkData[WorldManager::GetChunkKey(chunk->GetPosition().x, chunk->GetPosition().y, chunk->GetPosition().z)];
	renderData.m_VertexBuffer->Clear();
	renderData.m_IndexBuffer->Clear();
	renderData.m_VertexBufferTransparent->Clear();
	renderData.m_IndexBufferTransparent->Clear();
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				DrawBlock(chunk->GetBlock({ x,y,z }));
			}
		}
	}
	renderData.m_VertexBuffer->Bind();
	renderData.m_VertexBuffer->Allocate();
	renderData.m_VertexBufferTransparent->Bind();
	renderData.m_VertexBufferTransparent->Allocate();
}
