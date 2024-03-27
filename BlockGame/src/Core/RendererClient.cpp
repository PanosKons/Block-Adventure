#include "RendererClient.h"
#include "Common/World/WorldManager.h"
#include "Renderer.h"
#include "Entities/EntityManagerClient.h"
#include "Common/Math/StringConvertions.h"
#include "Logger.h"
#include "Client.h"
#include "Entities/EntityManagerClient.h"

#define ONEOVER16 0.0625f
void RenderBlockFace(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, Vector3<float> Position, Vector2<float> Size, Direction direction, unsigned char texture, float alpha)
{
	Vertex a;
	a.texId = 0.0f;
	float texcordsX = (texture % 16) / 16.0f;
	float texcordsY = (texture / 16) / 16.0f;
	a.texCords = { texcordsX, texcordsY };
	a.color = { 1,1,1,alpha };
	switch (direction)
	{
	case Direction::Backward:
	{
		a.normal = { 0,0,-1 };
		a.position = Position;
		vertexBuffer->Add(a);
		a.position.x += Size.x;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.y += Size.y;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.x = (float)Position.x;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
		break;
	}
	case Direction::Forward:
	{
		a.normal = { 0,0,1 };
		a.position = Position;
		a.position.x += Size.x;
		vertexBuffer->Add(a);
		a.position.x = Position.x;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.y += Size.y;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.x += Size.x;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
		break;
	}
	case Direction::Up:
	{
		a.normal = {0,1,0};
		a.position = Position;
		a.texCords = { texcordsX, texcordsY };
		vertexBuffer->Add(a);
		a.position.x += Size.x;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.z += Size.y;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.x = Position.x;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
		break;
	}
	case Direction::Down:
	{
		a.normal = { 0,-1,0 };
		a.position = Position;
		a.texCords = { texcordsX, texcordsY };
		vertexBuffer->Add(a);
		a.position.z += Size.x;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.x += Size.y;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.z = Position.z;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
		break;
	}
	case Direction::Left:
	{
		a.normal = { -1,0,0 };
		a.position = Position;
		a.position.z += Size.x;
		a.texCords = { texcordsX, texcordsY };
		vertexBuffer->Add(a);
		a.position.z = Position.z;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.y += Size.y;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.z += Size.x;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
		break;
	}
	case Direction::Right:
	{
		a.normal = { 1,0,0 };
		a.position = Position;
		vertexBuffer->Add(a);
		a.position.z += Size.x;
		a.texCords.x += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.y += Size.y;
		a.texCords.y += ONEOVER16;
		vertexBuffer->Add(a);
		a.position.z = Position.z;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
		break;
	}
	}
	indexBuffer->AddRectangle();
}
void RenderSelectorFace(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, Vector3<float> Position, Vector2<float> Size, Direction direction)
{
	Vertex a;
	a.texId = 1.0f;
	float texcordsX = 0.0f;
	float texcordsY = 0.0f;
	a.texCords = { texcordsX, texcordsY };
	a.color = { 1,1,1,1 };
	switch (direction)
	{
	case Direction::Backward:
	{
		a.position = Position;
		vertexBuffer->Add(a);
		a.position.x += Size.x;
		a.texCords.x += 1;
		vertexBuffer->Add(a);
		a.position.y += Size.y;
		a.texCords.y += 1;
		vertexBuffer->Add(a);
		a.position.x = (float)Position.x;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
		break;
	}
	case Direction::Forward:
	{
		a.position = Position;
		a.position.x += Size.x;
		vertexBuffer->Add(a);
		a.position.x = Position.x;
		a.texCords.x += 1;
		vertexBuffer->Add(a);
		a.position.y += Size.y;
		a.texCords.y += 1;
		vertexBuffer->Add(a);
		a.position.x += Size.x;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
		break;
	}
	case Direction::Up:
	{
		a.position = Position;
		a.texCords = { texcordsX, texcordsY };
		vertexBuffer->Add(a);
		a.position.x += Size.x;
		a.texCords.x += 1;
		vertexBuffer->Add(a);
		a.position.z += Size.y;
		a.texCords.y += 1;
		vertexBuffer->Add(a);
		a.position.x = Position.x;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
		break;
	}
	case Direction::Down:
	{
		a.position = Position;
		a.texCords = { texcordsX, texcordsY };
		vertexBuffer->Add(a);
		a.position.z += Size.x;
		a.texCords.x += 1;
		vertexBuffer->Add(a);
		a.position.x += Size.y;
		a.texCords.y += 1;
		vertexBuffer->Add(a);
		a.position.z = Position.z;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
		break;
	}
	case Direction::Left:
	{
		a.position = Position;
		a.position.z += Size.x;
		a.texCords = { texcordsX, texcordsY };
		vertexBuffer->Add(a);
		a.position.z = Position.z;
		a.texCords.x += 1;
		vertexBuffer->Add(a);
		a.position.y += Size.y;
		a.texCords.y += 1;
		vertexBuffer->Add(a);
		a.position.z += Size.x;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
		break;
	}
	case Direction::Right:
	{
		a.position = Position;
		vertexBuffer->Add(a);
		a.position.z += Size.x;
		a.texCords.x += 1;
		vertexBuffer->Add(a);
		a.position.y += Size.y;
		a.texCords.y += 1;
		vertexBuffer->Add(a);
		a.position.z = Position.z;
		a.texCords.x = texcordsX;
		vertexBuffer->Add(a);
		break;
	}
	}
	indexBuffer->AddRectangle();
}
float GetDistanceSquaredFromPlayer(Chunk* chunk)
{
	Vector3<float> Pos = { chunk->GetPosition().x * ChunkSize + (float)ChunkSize / 2,chunk->GetPosition().y * ChunkSize + (float)ChunkSize / 2,chunk->GetPosition().z * ChunkSize + (float)ChunkSize / 2 };
	Pos -= Vector::FloatVector(EntityManagerClient::GetPlayer().Position);
	return Pos.x * Pos.x + Pos.y * Pos.y + Pos.z * Pos.z;
}
void RendererClient::RenderBlock(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, Block& block)
{
	BlockProperties& bp = block.GetBlockProperties();
	Vertex a;
	a.texId = 0.0f;
	float alpha = block.GetBlockProperties().translucency;
	//if (block.data->RenderedSides & (unsigned char)1) {
		//	RenderBlockFace(vertexBuffer, indexBuffer, { (float)block.Position.x + 1.0f,(float)block.Position.y ,(float)block.Position.z }, { 1.0f,1.0f }, Direction::Right, bp.textureSides[0], alpha);
		//}
		//if (block.data->RenderedSides & (unsigned char)2) {
		//	RenderBlockFace(vertexBuffer, indexBuffer, { (float)block.Position.x,(float)block.Position.y,(float)block.Position.z }, { 1.0f,1.0f }, Direction::Left, bp.textureSides[1], alpha);
		//}
		//if (block.data->RenderedSides & (unsigned char)4) {
		//	RenderBlockFace(vertexBuffer, indexBuffer, { (float)block.Position.x,(float)block.Position.y,(float)block.Position.z + 1.0f }, { 1.0f,1.0f }, Direction::Forward, bp.textureSides[2], alpha);
		//}
		//if (block.data->RenderedSides & (unsigned char)8) {
		//	RenderBlockFace(vertexBuffer, indexBuffer, { (float)block.Position.x,(float)block.Position.y,(float)block.Position.z }, { 1.0f,1.0f }, Direction::Backward, bp.textureSides[3], alpha);
		//}
		//if (block.data->RenderedSides & (unsigned char)16) {
		//	RenderBlockFace(vertexBuffer, indexBuffer, { (float)block.Position.x,(float)block.Position.y + 1.0f ,(float)block.Position.z }, { 1.0f,1.0f }, Direction::Up, bp.textureSides[5], alpha);
		//}
		//if (block.data->RenderedSides & (unsigned char)32) {
		//	RenderBlockFace(vertexBuffer, indexBuffer, { (float)block.Position.x,(float)block.Position.y,(float)block.Position.z }, { 1.0f,1.0f }, Direction::Down, bp.textureSides[4], alpha);
		//}
	for (Face& face : Block::blockModels[bp.model].Faces)
	{
		if (block.data->RenderedSides & face.condition)
			RenderBlockFace(vertexBuffer, indexBuffer, face.position + Vector::FloatVector(block.Position), face.size, face.direction, bp.textureSides[face.textureIndex], alpha);
	}
}
void RendererClient::RenderChunk(Chunk* chunk)
{
	ASSERT(chunk, "Invalid chunk in rendering");
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
					if (!block.IsValid()) continue;
					if (block.GetBlockProperties().render == false) continue;
					VertexBuffer* vertexBuffer;
					block.GetBlockProperties().translucency != 1.0f ? vertexBuffer = &rd.Transparent.vertexBuffer : vertexBuffer = &rd.Solid.vertexBuffer;
					IndexBuffer* indexBuffer;
					block.GetBlockProperties().translucency != 1.0f ? indexBuffer = &rd.Transparent.indexBuffer : indexBuffer = &rd.Solid.indexBuffer;
					RenderBlock(vertexBuffer, indexBuffer, block);
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
void RendererClient::RenderWorld(World* world)
{
	std::vector<Chunk*> chunks;
	for (auto& [key, chunk] : *world->GetChunkMap())
	{
		chunks.push_back(chunk);
	}
	std::sort(chunks.begin(), chunks.end(), [](Chunk* first, Chunk* second) {return GetDistanceSquaredFromPlayer(first) > GetDistanceSquaredFromPlayer(second); });
	for (Chunk* chunk : chunks)
	{
		if (chunk->DontRender == true)
		{
			chunk->DontRender = false;
			continue;
		}
		RenderChunk(chunk);
	}
}
void RendererClient::RenderEntities()
{
	RenderBuilder::Begin(EntityRenderData);
	//Render players
	for (auto&[UUID, player] : EntityManagerClient::Players)
	{
		RenderBuilder::AddCube(EntityRenderData, { (float)(player->Position.x - player->Hitbox.x / 2),(float)(player->Position.y),(float)(player->Position.z - player->Hitbox.z / 2) }, Vector::FloatVector(player->Hitbox), { 0.6f,0.5f,0.6f,1.0f });
	}
	Block facingblock = EntityManagerClient::GetPlayer().GetFacingBlock();
	if (facingblock.data != nullptr)
	{
		for (Face& face : facingblock.GetBlockModel().Faces)
		{
			RenderSelectorFace(&EntityRenderData.vertexBuffer,&EntityRenderData.indexBuffer,face.position + Vector::FloatVector(facingblock.Position),face.size,face.direction);
		}
	}
	for (auto& [key, chunk] : *WorldManager::BaseWorld->GetChunkMap())
	{
		for (int i = 0; i < chunk->entities.size(); i++)
		{
			Entity& entity = chunk->entities[i];
			RenderBuilder::AddCube(EntityRenderData, { (float)(entity.Position.x - entity.Hitbox.x / 2),(float)(entity.Position.y),(float)(entity.Position.z - entity.Hitbox.z / 2) }, Vector::FloatVector(entity.Hitbox), { 0.8f,0.8f,0.8f,1.0f });
		}
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
		Entity* LookEntity = EntityManagerClient::GetPlayer().GetFacingEntity();
		if(LookEntity != nullptr)
			RenderBuilder::AddText(UIRenderData, "Facing entity:" + StringConvertions::ToString(LookEntity->Position.x) + "," + StringConvertions::ToString(LookEntity->Position.y) + "," + StringConvertions::ToString(LookEntity->Position.z), { 0.0f,Client::ScreenHeight - 160.0f });
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
			ItemStack& itemStack = EntityManagerClient::GetPlayer().Inventory[i];
			if (itemStack.GetCount() != 0)
			{
				unsigned char TextureId = Item::GetItemProperties(itemStack.GetItemType()).texture;
				RenderBuilder::AddSquare(UIRenderData,{ SlotPosition.x + 8, SlotPosition.y + 8 }, { SlotWidth - 16,SlotHeight - 16 }, { 1,1,1,1 }, { (TextureId % 16) / 16.0f, (TextureId / 16) / 16.0f }, { 1 / 16.0f, 1 / 16.0f }, 0.0f,BaseLayer - 0.2f);
			}
			SlotPosition.x += SlotWidth;
		}
		SlotPosition = { SlotsX,0.0f };
		for (int i = 0; i < InventorySize; i++)
		{
			if (EntityManagerClient::GetPlayer().Inventory[i].GetCount() > 1)
				RenderBuilder::AddText(UIRenderData,StringConvertions::ToString(EntityManagerClient::GetPlayer().Inventory[i].GetCount()), SlotPosition, BaseLayer - 0.1f);
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
	//Add Gui
	{
		if (EntityManagerClient::GetPlayer().IsGUIOpen == true)
		{
			Gui& gui = EntityManagerClient::GetPlayer().gui;
			RenderBuilder::AddSquare(UIRenderData, { (float)Client::ScreenWidth / 4, (float)Client::ScreenHeight / 4 }, { (float)Client::ScreenWidth / 2, (float)Client::ScreenHeight / 2 }, gui.Color, { 0,0 }, { 1,1 }, -1);
			for (auto& slot : gui.Slots)
			{
				if (slot.Active == true)
				{
					RenderBuilder::AddSquare(UIRenderData, { (float)Client::ScreenWidth / 4 + SlotsX * slot.Position.x + 2, (float)Client::ScreenHeight / 4 + SlotsX * slot.Position.y + 2 }, { SlotsX - 2,SlotsX -2 }, { 1.0f,1.0f,1.0f,1.0f }, { 0,0 }, {0,0}, -1);
				}
			}
		}
	}
	RenderBuilder::End(UIRenderData);
	Renderer::RenderCommand command;
	command.view = Renderer::View::UI;
	command.Depth = false;
	command.renderData = &UIRenderData;
	Renderer::AddCommand(command);
}