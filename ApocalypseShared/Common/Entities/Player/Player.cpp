#include "pch.h"
#include "Player.h"
#include "Input.h"
#include "Math/Ray.h"
#include "World/WorldConstants.h"
#include "Math/EngineMath.h"
#include "World/WorldManager.h"
Player::Player() {}

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
	BreakingBlock = block;
	TimeToBreak = (float)block.GetBlockProperties().hardness;
}
bool IsBlockSolid(Vector3<int> Position)
{
	Block block = WorldManager::BaseWorld->GetBlock({ Position.x,Position.y,Position.z });
	if (block.data != nullptr)
		return block.GetBlockId() != BLOCK_ID::Air;
	return true;
}
bool CheckCollision(Vector3<double> Position ,Vector3<double> Hitbox)
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
bool CheckCollision(Vector3<double> Position, Vector3<double> Hitbox, Vector3<int> block)
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
void Player::Update()
{
	Block facingblock = GetFacingBlock();
	DrawPlayer(facingblock);
	Renderer::DrawGeometry(*m_VertexBuffer, *m_IndexBuffer);
	if (isBreakingBlock)
	{
		if (facingblock != breakingBlock || Input::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) isBreakingBlock = false;
		if (Inventory[ActiveSlot].id == (int)ITEM_ID::Pickaxe && Inventory[ActiveSlot].type == TYPE::ITEM && breakingBlock.GetBlockProperties().tool == TOOL::Pickaxe)
		{
			TimeToBreak -= deltaTime * 60 * 12;
		}
		else
		{
			TimeToBreak -= deltaTime * 60;
		}
		if (TimeToBreak < 0)
		{
			int index = GetFirstAvaiableSlot((int)breakingBlock.GetBlockId(), TYPE::BLOCK);
			Inventory[index].id = (int)breakingBlock.GetBlockId();
			Inventory[index].type = TYPE::BLOCK;
			Inventory[index].count++;
			breakingBlock.OnBreak(BLOCK_ID::Air);
			isBreakingBlock = false;
		}
	}
	else if (Input::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && Playing)
	{
		MarkBlockToBreak();
	}
	BlockPlaceDelay -= deltaTime;
	if (Input::GetMouseState(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && Playing && BlockPlaceDelay < 0)
	{
		Block block = GetBlockToPlace();
		if (block.data == nullptr) return;
		if (CheckCollision(Position,Hitbox,block.Position) == true)
			return;
		if (Inventory[ActiveSlot].count > 0 && Inventory[ActiveSlot].type == TYPE::BLOCK)
		{
			block.OnBreak((BLOCK_ID)Inventory[ActiveSlot].id);
			Inventory[ActiveSlot].count--;
		}
		BlockPlaceDelay = 0.3f;
	}
	if (Input::GetKeyState(GLFW_KEY_G) == GLFW_PRESS && Playing)
	{
		godmode = !godmode;
	}

	if (Input::GetMouseState(GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS && Playing && godmode)
	{
		ItemStack& stack = GameManager::player->Inventory[GameManager::player->GetFirstAvaiableSlot((int)facingblock.GetBlockId(), TYPE::BLOCK)];
		stack.id = (int)facingblock.GetBlockId();
		stack.count += 1;
		stack.type = TYPE::BLOCK;
	}

	JumpCooldown -= deltaTime;

	if (!godmode)
	{
		Velocity.y -= 24.0f * deltaTime;
		grounded = CheckCollision({ Position.x , Position.y + Velocity.y * deltaTime, Position.z }, Hitbox);
	}
	else
	{
		if (Velocity.y < 0) Velocity.y = 0;
	}
	crouch = false;
	if (Input::GetKeyState(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && Playing)
	{
		speed = 2.0f;
		crouch = true;
	}
	else if (Input::GetKeyState(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && Playing)
	{
		speed = 6.0f;
		if (godmode) speed = 28.0f;
	}
	else
	{
		Speed = 4.0f;
	}
	if (Input::GetKeyState(GLFW_KEY_W) == GLFW_PRESS && Playing)
	{
		Velocity.x = Speed * cos(glm::radians(yaw));
		Velocity.z = Speed * sin(glm::radians(yaw));
	}
	else if (Input::GetKeyState(GLFW_KEY_S) == GLFW_PRESS && Playing)
	{
		Velocity.x = -speed * cos(glm::radians(yaw));
		Velocity.z = -speed * sin(glm::radians(yaw));
	}
	else if (Input::GetKeyState(GLFW_KEY_D) == GLFW_PRESS && Playing)
	{
		Velocity.x = speed * cos(glm::radians(yaw + 90));
		Velocity.z = speed * sin(glm::radians(yaw + 90));
	}
	else if (Input::GetKeyState(GLFW_KEY_A) == GLFW_PRESS && Playing)
	{
		Velocity.x = -speed * cos(glm::radians(yaw + 90));
		Velocity.z = -speed * sin(glm::radians(yaw + 90));
	}
	else
	{
		Velocity.x = 0;
		Velocity.z = 0;
	}

	if (Input::GetKeyState(GLFW_KEY_SPACE) == GLFW_PRESS && (grounded || godmode) && Playing && JumpCooldown <= 0)
	{
		crouch = false;
		Velocity.y = 7.2f;
		JumpCooldown += 0.4f;
	}

	if (crouch && grounded)
	{
		if (!CheckCollision({ Position.x + Velocity.x * deltaTime, Position.y +Velocity.y * deltaTime, Position.z }, Hitbox))
		{
			Velocity.x = 0;
		}
		if (!CheckCollision({ Position.x , Position.y + Velocity.y * deltaTime, Position.z + Velocity.z * deltaTime }, Hitbox))
		{
			Velocity.z = 0;
		}
	}
	if (CheckCollision({ Position.x + Velocity.x * deltaTime, Position.y, Position.z }, Hitbox))
	{
		Velocity.x = 0;
	}
	if (CheckCollision({ Position.x , Position.y + Velocity.y * deltaTime, Position.z }, Hitbox))
	{
		if (Velocity.y <= -6.0f) Health -= -(float)Velocity.y / 3.0f;
		Velocity.y = 0;
	}
	if (CheckCollision({ Position.x , Position.y, Position.z + Velocity.z * deltaTime }, Hitbox))
	{
		Velocity.z = 0;
	}

	//Apply the velocity to the position
	Position += Velocity * deltaTime;
}
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