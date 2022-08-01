#include "EntityManager.h"
#include "Common/Networking/Packet.h"
//static std::unique_ptr<VertexBuffer> vb;
//static std::unique_ptr<IndexBuffer> ib;
void EntityManager::Start()
{
	Players.reserve(MAX_PLAYERS);
	for (int i = 0; i < Players.capacity(); i++)
	{
		Players.push_back(nullptr);
	}
	//vb = std::make_unique<VertexBuffer>();
	//ib = std::make_unique<IndexBuffer>();
}
bool EntityManager::CheckCollision(Vector3<double> Position, Vector3<double> Hitbox)
{
	Vector3<int> Point1 = { (int)(Position.x - Hitbox.x / 2), (int)Position.y, (int)(Position.z - Hitbox.z / 2) };
	Vector3<int> Point2 = { (int)(Position.x + Hitbox.x / 2), (int)(Position.y + Hitbox.y), (int)(Position.z + Hitbox.z / 2) };
	for (int x = Point1.x; x <= Point2.x; x++)
	{
		for (int y = Point1.y; y <= Point2.y; y++)
		{
			for (int z = Point1.z; z <= Point2.z; z++)
			{
				if (IsBlockSolid({ x,y,z }))
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool EntityManager::CheckCollision(Vector3<double> Position, Vector3<double> Hitbox, Vector3<int> block)
{
	Vector3<int> Point1 = { (int)(Position.x - Hitbox.x / 2), (int)Position.y, (int)(Position.z - Hitbox.z / 2) };
	Vector3<int> Point2 = { (int)(Position.x + Hitbox.x / 2), (int)(Position.y + Hitbox.y), (int)(Position.z + Hitbox.z / 2) };
	for (int x = Point1.x; x <= Point2.x; x++)
	{
		for (int y = Point1.y; y <= Point2.y; y++)
		{
			for (int z = Point1.z; z <= Point2.z; z++)
			{
				if (block == Vector3<int>{x, y, z})
				{
					return true;
				}
			}
		}
	}
	return false;
}
/*void AddPlayerModel(VertexBuffer* vb, IndexBuffer* ib, Entity* player)
{
	Vector3<float> Position = Vector::FloatVector(Vector3<double>{ player->Position.x - player->Hitbox.x / 2,player->Position.y,player->Position.z - player->Hitbox.z / 2 });
	Vector3<float> Hitbox = Vector::FloatVector(player->Hitbox);
	Vertex a;
	a.texId = -1.0f;
	float alpha = 1.0f;
	a.color = { 0.9f,0.9f,0.9f,alpha };
	a.texCords = { 0, 0 };
	a.position = Position;
	vb->Add(a);
	a.position.x += Hitbox.x;
	a.texCords.x += 1.0f;
	vb->Add(a);
	a.position.y += Hitbox.y;
	a.texCords.y += 1.0f;
	vb->Add(a);
	a.position.x = Position.x;
	a.texCords.x = 0;
	vb->Add(a);
	a.color = { 0.85f,0.85f,0.85f,alpha };
	a.position = Position;
	a.texCords = { 0, 0 };
	a.position.x += Hitbox.x;
	vb->Add(a);
	a.position.z += Hitbox.z;
	a.texCords.x += 1.0f;
	vb->Add(a);
	a.position.y += Hitbox.y;
	a.texCords.y += 1.0f;
	vb->Add(a);
	a.position.z = Position.z;
	a.texCords.x = 0;
	vb->Add(a);
	a.color = { 0.75f,0.75f,0.75f,alpha };
	a.position = Position;
	a.texCords = { 0, 0 };
	a.position.z += Hitbox.z;
	a.position.x += Hitbox.x;
	vb->Add(a);
	a.position.x = Position.x;
	a.texCords.x += 1.0f;
	vb->Add(a);
	a.position.y += Hitbox.y;
	a.texCords.y += 1.0f;
	vb->Add(a);
	a.position.x += Hitbox.x;
	a.texCords.x = 0;
	vb->Add(a);
	a.color = { 0.8f,0.8f,0.8f,alpha };
	a.position = Position;
	a.position.z += Hitbox.z;
	a.texCords = { 0, 0 };
	vb->Add(a);
	a.position.z = Position.z;
	a.texCords.x += 1.0f;
	vb->Add(a);
	a.position.y += Hitbox.y;
	a.texCords.y += 1.0f;
	vb->Add(a);
	a.position.z += Hitbox.z;
	a.texCords.x = 0;
	vb->Add(a);
	a.color = { 0.7f,0.7f,0.7f,alpha };
	a.position = Position;
	a.texCords = { 0, 0 };
	vb->Add(a);
	a.position.z += Hitbox.z;
	a.texCords.x += 1.0f;
	vb->Add(a);
	a.position.x += Hitbox.x;
	a.texCords.y += 1.0f;
	vb->Add(a);
	a.position.z = Position.z;
	a.texCords.x = 0;
	vb->Add(a);
	a.color = { 1.0f,1.0f,1.0f,alpha };
	a.position = Position;
	a.position.y += Hitbox.y;
	a.texCords = { 0, 0 };
	vb->Add(a);
	a.position.x += Hitbox.x;
	a.texCords.x += 1.0f;
	vb->Add(a);
	a.position.z += Hitbox.z;
	a.texCords.y += 1.0f;
	vb->Add(a);
	a.position.x = Position.x;
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
			AddPlayerModel(vb.get(), ib.get(),player);
		}
	}
	vb->Bind();
	vb->Allocate();
	Renderer::DrawGeometry(*vb.get(), *ib.get());
}
void EntityManager::UpdatePlayer(int id, Vector3<double> Position)
{
	if (Players[id] == nullptr) AddPlayer(id);
	Players[id]->Position = Position;
}*/
void EntityManager::ShutDown()
{
	for (Player* player : Players)
	{
		delete player;
	}
}

Player* EntityManager::GetPlayer(int PlayerId)
{
	return Players[PlayerId];
}
