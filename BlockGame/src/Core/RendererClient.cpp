#include "RendererClient.h"
#include "Common/World/WorldManager.h"
#include "Renderer.h"
#include "Entities/EntityManagerClient.h"
#include "Common/Math/StringConvertions.h"
#include "Logger.h"
#include "Client.h"
#define ONEOVER16 0.0625f
void RendererClient::RenderWorld(World* world)
{
	auto ChunkMap = *(world->GetChunkMap());
	for (auto [key, chunk] : ChunkMap)
	{
		ChunkRenderData& rd = ChunkData[WorldManager::GetChunkKey(chunk->GetPosition())];
		if (rd.Populated == false)
		{
			rd.Solid.vertexBuffer.Clear();
			rd.Solid.indexBuffer.Clear();
			rd.Transparent.vertexBuffer.Clear();
			rd.Transparent.indexBuffer.Clear();
			for (int x = 0; x < ChunkSize; x++)
			{
				for (int y = 0; y < ChunkSize; y++)
				{
					for (int z = 0; z < ChunkSize; z++)
					{
						Block block = chunk->GetBlock({ x, y, z });
						block.data->RenderedSides = 63; // TEMPORARY
						VertexBuffer* vertexBuffer;
						block.GetTransparent() ? vertexBuffer = &rd.Transparent.vertexBuffer : vertexBuffer = &rd.Solid.vertexBuffer;
						IndexBuffer* indexBuffer;
						block.GetTransparent() ? indexBuffer = &rd.Transparent.indexBuffer : indexBuffer = &rd.Solid.indexBuffer;
						std::array<unsigned char, 6> arr = block.GetBlockProperties().textureSides;
						if (arr[0] == INVALID) continue;
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
				}
			}
			rd.Solid.vertexBuffer.Bind();
			rd.Solid.vertexBuffer.Allocate();
			rd.Transparent.vertexBuffer.Bind();
			rd.Transparent.vertexBuffer.Allocate();
		}
		rd.Populated = true;
		{
			Renderer::RenderCommand command;
			command.view = Renderer::View::Player;
			command.Depth = true;
			command.renderData = &rd.Solid;
			Renderer::AddCommand(command);
		}
		{
			Renderer::RenderCommand command;
			command.view = Renderer::View::Player;
			command.Depth = true;
			command.renderData = &rd.Transparent;
			Renderer::AddCommand(command);
		}
	}
}

void RendererClient::RenderEntities()
{

}

void RendererClient::RenderUI(double TimeStep)
{
	RenderBuilder::Begin(UIRenderData);
	RenderBuilder::AddSquare(UIRenderData, { 100.0f,100.0f }, { 100.0f,100.0f }, { 0.8f,0.6f,0.3f,1.0f }, { 0,0 }, {1,1}, -1);
	Vector3<double>& PlayerPosition = EntityManagerClient::GetPlayer().Position;
	RenderBuilder::AddText(UIRenderData, "Position:" + StringConvertions::ToString(PlayerPosition.x) + "," + StringConvertions::ToString(PlayerPosition.y) + "," + StringConvertions::ToString(PlayerPosition.z), {0.0f,Client::ScreenHeight - 40.0f});
	RenderBuilder::AddText(UIRenderData, "FPS: " + StringConvertions::ToString((int)(1 / TimeStep)), {0.0f,Client::ScreenHeight - 70.0f});
	RenderBuilder::End(UIRenderData);
	Renderer::RenderCommand command;
	command.view = Renderer::View::UI;
	command.Depth = false;
	command.renderData = &UIRenderData;
	Renderer::AddCommand(command);
}

void RendererClient::UpdateChunk(World* world, Chunk* chunk)
{
	ChunkData[WorldManager::GetChunkKey(chunk->GetPosition())].Populated = false;
}
