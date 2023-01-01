#include "RendererClient.h"
#include "Common/World/WorldManager.h"
#include "Renderer.h"
#include "Entities/EntityManagerClient.h"
#include "Common/Math/StringConvertions.h"
#include "Logger.h"
#include "Client.h"
#include "Entities/EntityManagerClient.h"
#define ONEOVER16 0.0625f
void RendererClient::RenderWorld(World* world)
{
	auto ChunkMap = *(world->GetChunkMap());
	for (auto [key, chunk] : ChunkMap)
	{
		ChunkRenderData& rd = ChunkData[WorldManager::GetChunkKey(chunk->GetPosition())];
		if (rd.Populated == false || chunk->MeshChanged == true)
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
						if (block.GetBlockProperties().render == false) continue;
						VertexBuffer* vertexBuffer;
						block.GetBlockProperties().transparent ? vertexBuffer = &rd.Transparent.vertexBuffer : vertexBuffer = &rd.Solid.vertexBuffer;
						IndexBuffer* indexBuffer;
						block.GetBlockProperties().transparent ? indexBuffer = &rd.Transparent.indexBuffer : indexBuffer = &rd.Solid.indexBuffer;
						std::array<unsigned char, 6> arr = block.GetBlockProperties().textureSides;
						Vertex a;
						a.texId = 0.0f;
						float alpha = 1.0f;
						//if (block.GetBlockId() == BLOCK_ID::Water) alpha = 0.4f;
						if (block.data->RenderedSides & (unsigned char)8) {
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
						if (block.data->RenderedSides & (unsigned char)1) {
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
						if (block.data->RenderedSides & (unsigned char)2) {
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
						if (block.data->RenderedSides & (unsigned char)32) {
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
						if (block.data->RenderedSides & (unsigned char)16) {
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
		chunk->MeshChanged = false;
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
	RenderBuilder::Begin(EntityRenderData);
	//Render players
	for (auto&[UUID, player] : EntityManagerClient::Players)
	{
		if(UUID != EntityManagerClient::GetPlayer().credentials.UUID)
			RenderBuilder::AddCube(EntityRenderData, Vector::FloatVector(player->Position), Vector::FloatVector(player->Hitbox), { 0.8f,1.0f,0.9f,1.0f });
	}
	//Render block selection
	#define WIDTH 0.02f
	#define OP_WIDTH 1.0f - WIDTH
	Block facingblock = EntityManagerClient::GetPlayer().GetFacingBlock();
	if (facingblock.data != nullptr)
	{
		Vector3<int> FacingBlockPosition = facingblock.Position;
		Vertex a;
		a.texCords = { 0,0 };
		a.texId = -1.0f;
		a.color = { 0.0f,0.0f,0.0f,1.0f };
		a.position = Vector::FloatVector(FacingBlockPosition);
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x -= 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.y += OP_WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += 0.02f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x -= 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.x += OP_WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x -= WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x -= WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z -= 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.y += OP_WIDTH;
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z -= 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z -= WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.z += OP_WIDTH;
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z -= WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.z += 1.0f;
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x -= 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.y += OP_WIDTH;
		a.position.z += 1.0f;
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x -= 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.z += 1.0f;
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x -= WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.x -= OP_WIDTH;
		a.position.z += 1.0f;
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x -= WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.z += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z -= 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.y += OP_WIDTH;
		a.position.z += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z -= 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.z += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z -= WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.z -= OP_WIDTH;
		a.position.z += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z -= WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.y += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z -= 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.x += OP_WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z -= 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z -= WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.z += OP_WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z -= WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.y += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x -= 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.z += OP_WIDTH;
		a.position.y += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x -= 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.y += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x -= WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
		a.position = Vector::FloatVector(FacingBlockPosition);
		a.position.x += OP_WIDTH;
		a.position.y += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x += WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.z += 1.0f;
		EntityRenderData.vertexBuffer.Add(a);
		a.position.x -= WIDTH;
		EntityRenderData.vertexBuffer.Add(a);
		EntityRenderData.indexBuffer.AddRectangle();
	}

	RenderBuilder::End(EntityRenderData);
	Renderer::RenderCommand command;
	command.view = Renderer::View::Player;
	command.Depth = true;
	command.renderData = &EntityRenderData;
	Renderer::AddCommand(command);
}

constexpr float SlotWidth = 64.0f;
constexpr float SlotHeight = 64.0f;
constexpr float BaseLayer = 10.0f;
constexpr float SlotsX = 64.0f;
constexpr float charHeight = 16.0f;
void RendererClient::RenderUI(double TimeStep)
{
	RenderBuilder::Begin(UIRenderData);
	//Info screen
	{
		RenderBuilder::AddSquare(UIRenderData, { (float)Client::ScreenWidth / 2, (float)Client::ScreenHeight / 2 }, { 20.0f,20.0f }, { 1.0f,1.0f,1.0f,1.0f }, { 0,0 }, { 1,1 }, 12);
		Vector3<double>& PlayerPosition = EntityManagerClient::GetPlayer().Position;
		RenderBuilder::AddText(UIRenderData, "Position:" + StringConvertions::ToString((int)PlayerPosition.x) + "," + StringConvertions::ToString((int)PlayerPosition.y) + "," + StringConvertions::ToString((int)PlayerPosition.z), { 0.0f,Client::ScreenHeight - 40.0f });
		RenderBuilder::AddText(UIRenderData, "FPS:" + StringConvertions::ToString((int)(1 / TimeStep)), { 0.0f,Client::ScreenHeight - 70.0f });
		Block LookBlock = EntityManagerClient::GetPlayer().GetFacingBlock();
		if (LookBlock.data != nullptr)
			RenderBuilder::AddText(UIRenderData, "Facing block:" + StringConvertions::ToString(LookBlock.Position.x) + "," + StringConvertions::ToString(LookBlock.Position.y) + "," + StringConvertions::ToString(LookBlock.Position.z), { 0.0f,Client::ScreenHeight - 100.0f });
	}
	//Health and Inventory
	{
		Vector2<float> SlotPosition = { SlotsX,0.0f};

		Vector2<float> HealthBarPosition = { SlotsX,10.0f + SlotHeight };
		RenderBuilder::AddSquare(UIRenderData, HealthBarPosition, { 320.0f,40.0f }, { 1.0f,0.0f,0.0f,1.0f }, { 0.0f,0.0f }, {1.0f,1.0f}, -1, BaseLayer);
		RenderBuilder::AddSquare(UIRenderData, HealthBarPosition, { 320.0f * EntityManagerClient::GetPlayer().Health / EntityManagerClient::GetPlayer().MaxHealth,40.0f }, { 0.0f,1.0f,0.0f,1.0f }, { 0.0f,0.0f }, { 1.0f,1.0f }, -1, BaseLayer);
		RenderBuilder::AddText(UIRenderData,StringConvertions::ToString((int)EntityManagerClient::GetPlayer().Health) + "/" + StringConvertions::ToString((int)EntityManagerClient::GetPlayer().MaxHealth), HealthBarPosition, BaseLayer + 0.3f);
		for (int i = 0; i < InventorySize; i++)
		{
			RenderBuilder::AddSquare(UIRenderData, SlotPosition, { SlotWidth,SlotHeight }, { 1,1,1,1 }, { 0.0f,0.0f }, { 1.0f,1.0f },14, BaseLayer - 0.3f);
			SlotPosition.x += SlotWidth;
		}
		SlotPosition = { SlotsX,0.0f };
		for (int i = 0; i < InventorySize; i++)
		{
			if (EntityManagerClient::GetPlayer().Inventory[i].count != 0)
			{
				//unsigned char a = GetTexture(EntityManagerClient::GetPlayer().Inventory[i].id, EntityManagerClient::GetPlayer().Inventory[i].type)[0];
				//RenderBuilder::AddSquare(UIRenderData,{ SlotPosition.x + 8, SlotPosition.y + 8 }, { SlotWidth - 16,SlotHeight - 16 }, { 1,1,1,1 }, { (a % 16) / 16.0f, (a / 16) / 16.0f }, { 1 / 16.0f, 1 / 16.0f }, 0.0f,BaseLayer - 0.2f);
			}
			SlotPosition.x += SlotWidth;
		}
		SlotPosition = { SlotsX,0.0f };
		for (int i = 0; i < InventorySize; i++)
		{
			if (EntityManagerClient::GetPlayer().Inventory[i].count > 1)
				RenderBuilder::AddText(UIRenderData,StringConvertions::ToString(EntityManagerClient::GetPlayer().Inventory[i].count), SlotPosition, BaseLayer - 0.1f);
			SlotPosition.x += SlotWidth;
		}
		SlotPosition = { SlotsX,0.0f };
		RenderBuilder::AddSquare(UIRenderData,{ SlotPosition.x + SlotWidth * EntityManagerClient::GetPlayer().ActiveSlot,SlotPosition.y }, { SlotWidth,SlotHeight }, { 1,1,1,1 }, { 0.0f,0.0f }, { 1.0f,1.0f }, 15, BaseLayer - 0.05f);
		if (EntityManagerClient::GetPlayer().IsGUIOpen)
		{
			Vector2<float> TypingText = { 0.0f,160.0f };
			RenderBuilder::AddText(UIRenderData, EntityManagerClient::GetPlayer().chatbox, TypingText);
			RenderBuilder::AddSquare(UIRenderData,{ 0.0f,0.0f}, { (float)Client::ScreenWidth, charHeight }, { 0.2f, 0.2f, 0.2f, 0.6f }, { 0.0f,0.0f }, { 1.0f,1.0f }, -1,BaseLayer - 0.1f);
		}
	}
	RenderBuilder::End(UIRenderData);
	Renderer::RenderCommand command;
	command.view = Renderer::View::UI;
	command.Depth = false;
	command.renderData = &UIRenderData;
	Renderer::AddCommand(command);
}