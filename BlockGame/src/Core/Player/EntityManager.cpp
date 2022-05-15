#include "EntityManager.h"
#include "Renderer.h"
#include "Serializer.h"
static std::unique_ptr<VertexBuffer> vb;
static std::unique_ptr<IndexBuffer> ib;
void EntityManager::Start()
{
	Players.reserve(MAX_PLAYERS);
	for (int i = 0; i < Players.capacity(); i++)
	{
		Players.push_back(nullptr);
	}
	vb = std::make_unique<VertexBuffer>();
	ib = std::make_unique<IndexBuffer>();
}
void EntityManager::Render()
{
	vb->Clear();
	ib->Clear();
	for (Entity* entity : Entities)
	{
		Renderer::DrawSquare(*vb.get(), *ib.get(), { (float)entity->Position.x,(float)entity->Position.y,(float)entity->Position.z }, { 3,3 }, { 1,1,1,1 }, { 0,0 }, { 1,1 }, -1);
	}
	for (Entity* player : Players)
	{
		if(player != nullptr)
			Renderer::DrawSquare(*vb.get(), *ib.get(), { (float)player->Position.x,(float)player->Position.y,(float)player->Position.z }, { 3,3 }, { 1,1,1,1 }, { 0,0 }, { 1,1 }, -1);
	}
	vb->Bind();
	vb->Allocate();
	Renderer::DrawGeometry(*vb.get(), *ib.get());
}
void EntityManager::ShutDown()
{
	for (Entity* entity : Players)
	{
		delete entity;
	}
	for (Entity* entity : Entities)
	{
		delete entity;
	}
}

void EntityManager::AddPlayer(int id)
{
	Entity* player = new Entity();
	Players[id] = player;
}

void EntityManager::UpdatePlayer(int id, Vector3<double> Position)
{
	if (Players[id] == nullptr) AddPlayer(id);
	Players[id]->Position = Position;
}
