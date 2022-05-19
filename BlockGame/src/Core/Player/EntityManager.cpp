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
void AddCube(VertexBuffer* vb,IndexBuffer* ib, Entity* player)
{
	Vertex a;
	a.texId = -1.0f;
	float alpha = 1.0f;
	a.color = { 0.9f,0.9f,0.9f,alpha };
	a.texCords = { 0, 0 };
	a.position = Vector::FloatVector(player->Position);
	vb->Add(a);
	a.position.x += 1.0f;
	a.texCords.x += 1.0f;
	vb->Add(a);
	a.position.y += 1.0f;
	a.texCords.y += 1.0f;
	vb->Add(a);
	a.position.x = (float)player->Position.x;
	a.texCords.x = 0;
	vb->Add(a);
	a.color = { 0.85f,0.85f,0.85f,alpha };
	a.position = Vector::FloatVector(player->Position);
	a.texCords = { 0, 0 };
	a.position.x += 1.0f;
	vb->Add(a);
	a.position.z += 1.0f;
	a.texCords.x += 1.0f;
	vb->Add(a);
	a.position.y += 1.0f;
	a.texCords.y += 1.0f;
	vb->Add(a);
	a.position.z = (float)player->Position.z;
	a.texCords.x = 0;
	vb->Add(a);
	a.color = { 0.75f,0.75f,0.75f,alpha };
	a.position = Vector::FloatVector(player->Position);
	a.texCords = { 0, 0 };
	a.position.z += 1.0f;
	a.position.x += 1.0f;
	vb->Add(a);
	a.position.x = (float)player->Position.x;
	a.texCords.x += 1.0f;
	vb->Add(a);
	a.position.y += 1.0f;
	a.texCords.y += 1.0f;
	vb->Add(a);
	a.position.x += 1.0f;
	a.texCords.x = 0;
	vb->Add(a);
	a.color = { 0.8f,0.8f,0.8f,alpha };
	a.position = Vector::FloatVector(player->Position);
	a.position.z += 1.0f;
	a.texCords = { 0, 0 };
	vb->Add(a);
	a.position.z = (float)player->Position.z;
	a.texCords.x += 1.0f;
	vb->Add(a);
	a.position.y += 1.0f;
	a.texCords.y += 1.0f;
	vb->Add(a);
	a.position.z += 1.0f;
	a.texCords.x = 0;
	vb->Add(a);
	a.color = { 0.7f,0.7f,0.7f,alpha };
	a.position = Vector::FloatVector(player->Position);
	a.texCords = { 0, 0 };
	vb->Add(a);
	a.position.z += 1.0f;
	a.texCords.x += 1.0f;
	vb->Add(a);
	a.position.x += 1.0f;
	a.texCords.y += 1.0f;
	vb->Add(a);
	a.position.z = (float)player->Position.z;
	a.texCords.x = 0;
	vb->Add(a);
	a.color = { 1.0f,1.0f,1.0f,alpha };
	a.position = Vector::FloatVector(player->Position);
	a.position.y += 1.0f;
	a.texCords = { 0, 0 };
	vb->Add(a);
	a.position.x += 1.0f;
	a.texCords.x += 1.0f;
	vb->Add(a);
	a.position.z += 1.0f;
	a.texCords.y += 1.0f;
	vb->Add(a);
	a.position.x = (float)player->Position.x;
	a.texCords.x = 0;
	vb->Add(a);
	ib->AddCuboid(63);
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
		if (player != nullptr)
		{
			AddCube(vb.get(), ib.get(),player);
		}
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
