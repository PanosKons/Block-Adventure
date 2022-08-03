#include "pch.h"
#include "Player.h"
#include "Math/Ray.h"
#include "World/WorldConstants.h"
#include "Math/EngineMath.h"
#include "World/WorldManager.h"
Player::Player()
{
	Position = { 260,80,260 };
	Hitbox = { 0.6, 1.8 ,0.6 };
	Velocity = { 0,0,0 };
	MaxHealth = 100.0f;
	Health = 100.0f;
	Speed = 5.0f;
	Pitch = 30.0f;
	Yaw = 30.0f;
	Grounded = false;
	ActiveSlot = 0;
	Inventory = {};
	TimeToBreak = 0.0f;
	BreakingBlockPosition = { 0,0,0 };
	IsBreakingBlock = false;
	BlockPlaceDelay = 0.3f;
	JumpCooldown = 0.0f;
	Crouch = false;
	Godmode = true;
}
Player::~Player() {}

int Player::GetFirstAvaiableSlot(int id,TYPE type)
{
	for (unsigned int i = 0; i < Inventory.size(); i++)
	{
		if (Inventory[i].id == id && Inventory[i].type == type) return i;
	}
	for (unsigned int i = 0; i < Inventory.size(); i++)
	{
		if (Inventory[i].count == 0) return i;
	}
	return -1;
}
Vector3<double> Player::GetLookPosition()
{
	return { Position.x,Position.y + 1.6, Position.z };
}
Block Player::GetFacingBlock()
{
	Ray ray(GetLookPosition(), Pitch, Yaw);
	Block block = WorldManager::BaseWorld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	while (true)
	{
		if (block.data == nullptr) break;
		if (block.GetBlockId() != BLOCK_ID::Air) break;
		ray.step(0.1f);
		if (ray.getLength() > 5.9f)
			return Block();
		block = WorldManager::BaseWorld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	}
	return block;
}
Block Player::GetBlockToPlace()
{
	Ray ray(GetLookPosition(), Pitch, Yaw);
	Block block = WorldManager::BaseWorld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	Block lastBlock = Block();
	while (true)
	{
		if (block.data == nullptr) break;
		if (block.GetBlockId() != BLOCK_ID::Air) break;
		ray.step(0.1f);
		if (ray.getLength() > 5.9f)
			return Block();
		lastBlock = block;
		block = WorldManager::BaseWorld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	}
	return lastBlock;
}
void Player::MarkBlockToBreak()
{
	Block block = GetFacingBlock();
	if (block.data == nullptr) return;
	IsBreakingBlock = true;
	BreakingBlockPosition = block.Position;
	TimeToBreak = (float)block.GetBlockProperties().hardness;
}
void Player::Update()
{}
/*
#define WIDTH 0.02f
#define OP_WIDTH 1.0f - WIDTH
void Player::DrawPlayer(Block facingblock)
{
	m_VertexBuffer->Clear();
	m_IndexBuffer->Clear();
	if (facingblock.data == nullptr) return;
	Vector3<int> FacingBlockPosition = facingblock.Position;
	Vertex a;
	a.texCords = { 0,0 };
	a.texId = -1.0f;
	a.color = { 0.0f,0.0f,0.0f,1.0f };
	a.position = Vector::FloatVector(FacingBlockPosition);
	m_VertexBuffer->Add(a);
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.y += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.x -= 1.0f;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.y += OP_WIDTH;
	m_VertexBuffer->Add(a);
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.y += 0.02f;
	m_VertexBuffer->Add(a);
	a.position.x -= 1.0f;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.x += OP_WIDTH;
	m_VertexBuffer->Add(a);
	a.position.x += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.y += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x -= WIDTH;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	m_VertexBuffer->Add(a);
	a.position.x += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.y += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x -= WIDTH;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.y += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.z -= 1.0f;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.y += OP_WIDTH;
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.y += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.z -= 1.0f;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.y += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z -= WIDTH;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.z += OP_WIDTH;
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.y += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z -= WIDTH;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.z += 1.0f;
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x -= 1.0f;
	m_VertexBuffer->Add(a);
	a.position.y += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.y += OP_WIDTH;
	a.position.z += 1.0f;
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x -= 1.0f;
	m_VertexBuffer->Add(a);
	a.position.y += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.z += 1.0f;
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x -= WIDTH;
	m_VertexBuffer->Add(a);
	a.position.y += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x += WIDTH;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.x -= OP_WIDTH;
	a.position.z += 1.0f;
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x -= WIDTH;
	m_VertexBuffer->Add(a);
	a.position.y += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x += WIDTH;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.z += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z -= 1.0f;
	m_VertexBuffer->Add(a);
	a.position.y += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.z += 1.0f;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.y += OP_WIDTH;
	a.position.z += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z -= 1.0f;
	m_VertexBuffer->Add(a);
	a.position.y += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.z += 1.0f;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.z += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z -= WIDTH;
	m_VertexBuffer->Add(a);
	a.position.y += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z += WIDTH;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.z -= OP_WIDTH;
	a.position.z += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z -= WIDTH;
	m_VertexBuffer->Add(a);
	a.position.y += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z += WIDTH;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	m_VertexBuffer->Add(a);
	a.position.z += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.z -= 1.0f;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.x += OP_WIDTH;
	m_VertexBuffer->Add(a);
	a.position.z += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.z -= 1.0f;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	m_VertexBuffer->Add(a);
	a.position.z += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z -= WIDTH;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.z += OP_WIDTH;
	m_VertexBuffer->Add(a);
	a.position.z += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z -= WIDTH;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.y += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.x -= 1.0f;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.z += OP_WIDTH;
	a.position.y += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.z += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.x -= 1.0f;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.y += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.z += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x -= WIDTH;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	a.position = Vector::FloatVector(FacingBlockPosition);
	a.position.x += OP_WIDTH;
	a.position.y += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x += WIDTH;
	m_VertexBuffer->Add(a);
	a.position.z += 1.0f;
	m_VertexBuffer->Add(a);
	a.position.x -= WIDTH;
	m_VertexBuffer->Add(a);
	m_IndexBuffer->AddRectangle();
	m_VertexBuffer->Bind();
	m_VertexBuffer->Allocate();
}
*/