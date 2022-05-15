#include <Engine.h>
#include "Player.h"
#include "GameManager.h"
#include "Input.h"
#include "Math/Ray.h"
#include "GlobalVariables.h"
#include "SavingData.h"
#include "Renderer.h"
#include "Timer.h"
#include "Math/EngineMath.h"
#include "Networking.h"
Player::Player()
	:ActiveSlot(0), Inventory(), mainCamera(), Velocity(0), Position({ 1065.0,80.0,1065.0 }), Hitbox({ 0.6, 1.8 ,0.6 })
{
	SavingData::LoadPlayer(this);
	Input::SetCursorCallback([](GLFWwindow* window, double xpos, double ypos) {GameManager::player->CursorMoved(xpos, ypos); });
	//Code to setup camera //identical to cursorMoved function
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	mainCamera.cameraFront = glm::normalize(front);

	m_VertexBuffer = std::make_unique<VertexBuffer>();
	m_IndexBuffer = std::make_unique<IndexBuffer>();
}
Player::~Player()
{
	SavingData::SavePlayer(this);
}
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
Block* Player::GetFacingBlock()
{
	Ray ray(mainCamera.cameraPos, pitch, yaw);
	Block* block = GameManager::Overworld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	while (true)
	{
		if (block == nullptr) break;
		if (block->GetBlockId() != BLOCK_ID::Air) break;
		ray.step(0.1f);
		if (ray.getLength() > 5.9f)
			return nullptr;
		block = GameManager::Overworld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	}
	return block;
}
Block* Player::GetBlockToPlace()
{
	Ray ray(mainCamera.cameraPos, pitch, yaw);
	Block* block = GameManager::Overworld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	while (true)
	{
		if (block == nullptr) return nullptr;
		if ((block->RenderedSides & (255 - 64)) != 0) break;
		ray.step(0.1f);
		if (ray.getLength() > 5.9f)
			return nullptr;
		block = GameManager::Overworld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	}
	return GameManager::Overworld->GetBlock({ (int)ray.getLast().x, (int)ray.getLast().y, (int)ray.getLast().z });
}
void Player::MarkBlockToBreak()
{
	Block* block = GetFacingBlock();
	if (block == nullptr) return;
	isBreakingBlock = true;
	breakingBlock = block;
	TimeToBreak = (float)block->GetBlockProperties().hardness;
}
void Player::CursorMoved(double xpos, double ypos)
{
	if (!Playing) return;
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;
	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (yaw < -180)
		yaw += 360;
	if (yaw > 180)
		yaw -= 360;
	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	mainCamera.cameraFront = glm::normalize(front);
}
bool IsBlockSolid(Vector3<int> Position)
{
	Block* block = GameManager::Overworld->GetBlock({ Position.x,Position.y,Position.z });
	if (block != nullptr)
		return block->GetBlockId() != BLOCK_ID::Air;
	return true;
}
bool CheckCollision(Vector3<double> Position ,Vector3<double> Hitbox)
{
	Vector3<int> Point1 = { (int)(Position.x - Hitbox.x / 2), (int)Position.y, (int)(Position.z - Hitbox.z / 2) };
	Vector3<int> Point2 = { (int)(Position.x + Hitbox.x / 2), (int)(Position.y + Hitbox.y), (int)(Position.z + Hitbox.z / 2) };
	bool Collision = false;
	for (int x = Point1.x; x <= Point2.x; x++)
	{
		for (int y = Point1.y; y <= Point2.y; y++)
		{
			for (int z = Point1.z; z <= Point2.z; z++)
			{
				if (IsBlockSolid({ x,y,z }))
				{
					Collision = true;
					break;
				}
			}
		}
	}
	return Collision;
}
void Player::Update(float deltaTime)
{
	deltaTime = Math::Min(deltaTime, 0.05f);
	Block* facingblock = GetFacingBlock();
	DrawPlayer(facingblock);
	Renderer::DrawGeometry(*m_VertexBuffer, *m_IndexBuffer);
	if (isBreakingBlock)
	{
		if (facingblock != breakingBlock || Input::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) isBreakingBlock = false;
		if (Inventory[ActiveSlot].id == (int)ITEM_ID::Pickaxe && Inventory[ActiveSlot].type == TYPE::ITEM && breakingBlock->GetBlockProperties().tool == TOOL::Pickaxe)
		{
			TimeToBreak -= deltaTime * 60 * 12;
		}
		else
		{
			TimeToBreak -= deltaTime * 60;
		}
		if (TimeToBreak < 0)
		{
			int index = GetFirstAvaiableSlot((int)breakingBlock->GetBlockId(), TYPE::BLOCK);
			Inventory[index].id = (int)breakingBlock->GetBlockId();
			Inventory[index].type = TYPE::BLOCK;
			Inventory[index].count++;
			breakingBlock->OnBreak(BLOCK_ID::Air);
			isBreakingBlock = false;
			//Send
			std::array<char, sizeof(Vector3<int>) + sizeof(BLOCK_ID)> buffer = std::array<char, sizeof(Vector3<int>) + sizeof(BLOCK_ID)>();
			int* p = (int*)buffer.data();
			*(Vector3<int>*)p = breakingBlock->Position;
			*(p + sizeof(Vector3<int>)) = (int)BLOCK_ID::Air;
			Networking::SendData(PACKET_ID::BreakBlock,buffer.data(), sizeof(breakingBlock->Position) + sizeof(BLOCK_ID));
		}
	}
	else if (Input::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && Playing)
	{
		MarkBlockToBreak();
	}
	BlockPlaceDelay -= deltaTime;
	if (Input::GetMouseState(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && Playing && BlockPlaceDelay < 0)
	{
		Block* block = GetBlockToPlace();
		if (block == nullptr) return;
		if (block->Position.x == (int)mainCamera.cameraPos.x && (block->Position.y == (int)mainCamera.cameraPos.y || block->Position.y == (int)mainCamera.cameraPos.y - 1) && block->Position.z == (int)mainCamera.cameraPos.z) return;
		if (Inventory[ActiveSlot].count > 0 && Inventory[ActiveSlot].type == TYPE::BLOCK)
		{
			block->OnBreak((BLOCK_ID)Inventory[ActiveSlot].id);
			Inventory[ActiveSlot].count--;

			//Send
			std::array<char, sizeof(Vector3<int>) + sizeof(BLOCK_ID)> buffer = std::array<char, sizeof(Vector3<int>) + sizeof(BLOCK_ID)>();
			int* p = (int*)buffer.data();
			*(Vector3<int>*)p = block->Position;
			*(p + sizeof(Vector3<int>) / sizeof(int)) = Inventory[ActiveSlot].id;
			Networking::SendData(PACKET_ID::BreakBlock, buffer.data(), sizeof(block->Position) + sizeof(BLOCK_ID));
		}
		else
		{
			IClickable* cb = dynamic_cast<IClickable*>(facingblock);
			if (cb) cb->OnClick();
		}
		BlockPlaceDelay = 0.3f;
	}
	if (Input::GetKeyState(GLFW_KEY_G) == GLFW_PRESS && Playing)
	{
		godmode = !godmode;
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
		speed = 4.0f;
	}
	if (Input::GetKeyState(GLFW_KEY_W) == GLFW_PRESS && Playing)
	{
		Velocity.x = speed * cos(glm::radians(yaw));
		Velocity.z = speed * sin(glm::radians(yaw));
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
		Velocity.y = 0;
	}
	if (CheckCollision({ Position.x , Position.y, Position.z + Velocity.z * deltaTime }, Hitbox))
	{
		Velocity.z = 0;
	}

	//Apply the velocity to the position
	Position += Velocity * deltaTime;
	mainCamera.cameraPos = glm::vec3(Position.x, Position.y + 1.6f, Position.z);
	Vector3<int> chunkpos = { ((int)Position.x) / ChunkSize, ((int)Position.y) / ChunkSize,((int)Position.z) / ChunkSize };
	if (chunkpos != ChunkPosition)
	{
		GameManager::Overworld->LoadPlayerChunks(chunkpos, 1);
		GameManager::Overworld->UnLoadPlayerChunks(chunkpos, 1);
		GameManager::Overworld->SubmitChunkChanges();
		ChunkPosition = chunkpos;
	}
}
#define WIDTH 0.02f
#define OP_WIDTH 1.0f - WIDTH
void Player::DrawPlayer(Block* facingblock)
{
	m_VertexBuffer->Clear();
	m_IndexBuffer->Clear();
	if (facingblock == nullptr) return;
	Vector3<int> FacingBlockPosition = facingblock->Position;
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
