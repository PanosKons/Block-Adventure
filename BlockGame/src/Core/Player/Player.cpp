#include <Engine.h>
#include "Player.h"
#include "GameManager.h"
#include "Input.h"
#include "Math/Ray.h"
#include <functional>
#include <iostream>
#include "GlobalVariables.h"
Player::Player()
	:ActiveSlot(0), Inventory(), mainCamera(), grounded(false), Velocity(0), Position({ 20.5f,63.0f,20.5f }), Hitbox({ Position.x - 0.3f, Position.y ,Position.z - 0.3f }, { 0.7f,2,0.7f })
{
	Input::SetCursorCallback([](GLFWwindow* window, double xpos, double ypos) {GameManager::player->CursorMoved(xpos, ypos); });
}
int Player::GetFirstAvaiableSlot(BLOCK_ID id)
{
	for (unsigned int i = 0; i < Inventory.size(); i++)
	{
		if (Inventory[i].id == id) return i;
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
static int value = 0;
bool IsBlockSolid(Vector3<int> Position)
{
	Block* block = GameManager::Overworld->GetBlock(Position);
	if (block != nullptr)
		return block->GetBlockId() != BLOCK_ID::Air;
	return true;
}
void Player::Update(float deltaTime)
{
	if (isBreakingBlock)
	{
		if (GetFacingBlock() != breakingBlock || Input::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) isBreakingBlock = false;
		TimeToBreak -= deltaTime * 60;
		if (TimeToBreak < 0)
		{
			int index = GetFirstAvaiableSlot(breakingBlock->GetBlockId());
			Inventory[index].id = breakingBlock->GetBlockId();
			Inventory[index].count++;
			breakingBlock->OnBreak(BLOCK_ID::Air);
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
		Block* block = GetBlockToPlace();
		if (block == nullptr) return;
		if (block->Position.x == (int)mainCamera.cameraPos.x && (block->Position.y == (int)mainCamera.cameraPos.y || block->Position.y == (int)mainCamera.cameraPos.y - 1) && block->Position.z == (int)mainCamera.cameraPos.z) return;
		if (Inventory[ActiveSlot].count > 0)
		{
			block->OnBreak(Inventory[ActiveSlot].id);
			Inventory[ActiveSlot].count--;
		}
		BlockPlaceDelay = 0.3f;
	}
	JumpCooldown -= deltaTime;
	grounded = IsBlockSolid({ (int)(Position.x - 0.27f), (int)Position.y, (int)(Position.z - 0.27f) })
		|| IsBlockSolid({ (int)(Position.x + 0.27f), (int)Position.y, (int)(Position.z - 0.27f) })
		|| IsBlockSolid({ (int)(Position.x + 0.27f), (int)Position.y, (int)(Position.z + 0.27f) })
		|| IsBlockSolid({ (int)(Position.x - 0.27f), (int)Position.y, (int)(Position.z + 0.27f) });
	if (!grounded)
	{
		Velocity.y -= 9.81f * deltaTime;
	}
	else
	{
		if (Velocity.y < 0) Velocity.y = 0;
	}
	if (Input::GetKeyState(GLFW_KEY_SPACE) == GLFW_PRESS && grounded && Playing && JumpCooldown <= 0)
	{
		Velocity.y = 8.5f;
		JumpCooldown += 0.4f;
	}
	if (Input::GetKeyState(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && Playing)
	{
		speed = 6.0f;
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
	bool XCollisionMinus = IsBlockSolid({ (int)(Position.x - 0.31f), (int)(Position.y + 0.2f), (int)(Position.z - 0.28f) })
		|| IsBlockSolid({ (int)(Position.x - 0.31f), (int)(Position.y + 0.2f), (int)(Position.z + 0.28f) })
		|| IsBlockSolid({ (int)(Position.x - 0.31f), (int)(Position.y + 1.1f), (int)(Position.z - 0.28f) })
		|| IsBlockSolid({ (int)(Position.x - 0.31f), (int)(Position.y + 1.1f), (int)(Position.z + 0.28f) });
	bool XCollisionPos = IsBlockSolid({ (int)(Position.x + 0.31f), (int)(Position.y + 0.2f), (int)(Position.z - 0.28f) })
		|| IsBlockSolid({ (int)(Position.x + 0.31f), (int)(Position.y + 0.2f), (int)(Position.z + 0.28f) })
		|| IsBlockSolid({ (int)(Position.x + 0.31f), (int)(Position.y + 1.1f), (int)(Position.z - 0.28f) })
		|| IsBlockSolid({ (int)(Position.x + 0.31f), (int)(Position.y + 1.1f), (int)(Position.z + 0.28f) });
	bool ZCollisionMinus = IsBlockSolid({ (int)(Position.x - 0.28f), (int)(Position.y + 0.2f), (int)(Position.z - 0.31f) })
		|| IsBlockSolid({ (int)(Position.x + 0.28f), (int)(Position.y + 0.2f), (int)(Position.z - 0.31f) })
		|| IsBlockSolid({ (int)(Position.x - 0.28f), (int)(Position.y + 1.1f), (int)(Position.z - 0.31f) })
		|| IsBlockSolid({ (int)(Position.x + 0.28f), (int)(Position.y + 1.1f), (int)(Position.z - 0.31f) });
	bool ZCollisionPos = IsBlockSolid({ (int)(Position.x - 0.28f), (int)(Position.y + 0.2f), (int)(Position.z + 0.31f) })
		|| IsBlockSolid({ (int)(Position.x + 0.28f), (int)(Position.y + 0.2f), (int)(Position.z + 0.31f) })
		|| IsBlockSolid({ (int)(Position.x - 0.28f), (int)(Position.y + 1.1f), (int)(Position.z + 0.31f) })
		|| IsBlockSolid({ (int)(Position.x + 0.28f), (int)(Position.y + 1.1f), (int)(Position.z + 0.31f) });
	bool YCollisionPos = IsBlockSolid({ (int)(Position.x - 0.27f), (int)(Position.y + 1.9f), (int)(Position.z + 0.27f) })
		|| IsBlockSolid({ (int)(Position.x + 0.27f), (int)(Position.y + 1.9f), (int)(Position.z + 0.27f) })
		|| IsBlockSolid({ (int)(Position.x + 0.27f), (int)(Position.y + 1.9f), (int)(Position.z - 0.27f) })
		|| IsBlockSolid({ (int)(Position.x - 0.27f), (int)(Position.y + 1.9f), (int)(Position.z - 0.27f) });
	if (XCollisionPos && Velocity.x > 0) Velocity.x = 0;
	if (XCollisionMinus && Velocity.x < 0) Velocity.x = 0;
	if (ZCollisionPos && Velocity.z > 0) Velocity.z = 0;
	if (ZCollisionMinus && Velocity.z < 0) Velocity.z = 0;
	if (YCollisionPos && Velocity.y > 0) Velocity.y = 0;
	Position += Velocity * deltaTime;
	mainCamera.cameraPos = glm::vec3(Position.x, Position.y + 1.8f, Position.z);
}