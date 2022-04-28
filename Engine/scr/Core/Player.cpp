#include "Player.h"
#include "GameManager.h"
#include "Input.h"
#include "../Math/Ray.h"
#include <functional>
#include <iostream>
#include "GlobalVariables.h"
Player::Player()
	:mainCamera(), grounded(false), Velocity(), Position({ 20.5f,63.0f,20.5f }), Hitbox({ Position.x - 0.3f, Position.y ,Position.z - 0.3f }, { 0.7f,2,0.7f })
{
	Input::SetCursorCallback([](GLFWwindow* window, double xpos, double ypos) {GameManager::player->CursorMoved(xpos, ypos); });
	Input::SetMouseCallback([](GLFWwindow* window, int button, int action, int mods) {GameManager::player->MouseButton(button, action); });
}
Block* Player::GetFacingBlock()
{
	Ray ray(mainCamera.cameraPos, pitch, yaw);
	Block* block = GameManager::Overworld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	while (true)
	{
		if (block == nullptr) break;
		if (block->id != BLOCK_ID::Air) break;
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
		if (block->id != BLOCK_ID::Air) break;
		ray.step(0.1f);
		if (ray.getLength() > 5.9f)
			return nullptr;
		block = GameManager::Overworld->GetBlock({ (int)ray.getEnd().x, (int)ray.getEnd().y, (int)ray.getEnd().z });
	}
	return GameManager::Overworld->GetBlock({ (int)ray.getLast().x, (int)ray.getLast().y, (int)ray.getLast().z });
}
void Player::MouseButton(int button, int action)
{
	if (!Playing) return;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		Block* block = GetFacingBlock();
		if (block == nullptr) return;
		block->ChangeState(BLOCK_ID::Air);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		Block* block = GetBlockToPlace();
		if (block == nullptr) return;
		if (block->Transform.x == (int)mainCamera.cameraPos.x && (block->Transform.y == (int)mainCamera.cameraPos.y || block->Transform.y == (int)mainCamera.cameraPos.y - 1) && block->Transform.z == (int)mainCamera.cameraPos.z) return;
		block->ChangeState(BLOCK_ID::Cobblestone);
	}
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
void Player::Update(float deltaTime)
{
	grounded = GameManager::Overworld->GetBlock({ (int)(Position.x - 0.29f), (int)Position.y, (int)(Position.z - 0.29f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x + 0.29f), (int)Position.y, (int)(Position.z - 0.29f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x + 0.29f), (int)Position.y, (int)(Position.z + 0.29f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x - 0.29f), (int)Position.y, (int)(Position.z + 0.29f) })->id != BLOCK_ID::Air;
	if (!grounded)
	{
		Velocity.y -= 0.005f * deltaTime;
	}
	else
	{
		Velocity.y = 0;
	}
	if (Input::GetKeyState(GLFW_KEY_SPACE) == GLFW_PRESS && grounded && Playing)
	{
		Velocity.y = 5.0f * deltaTime;
	}
	if (Input::GetKeyState(GLFW_KEY_W) == GLFW_PRESS && Playing)
	{
		Velocity.x = 3.0f * cos(glm::radians(yaw)) * deltaTime;
		Velocity.z = 3.0f * sin(glm::radians(yaw)) * deltaTime;
	}
	else
	{
		Velocity.x = 0;
		Velocity.z = 0;
	}
	bool XCollisionMinus = GameManager::Overworld->GetBlock({ (int)(Position.x - 0.3f), (int)(Position.y + 0.1f), (int)(Position.z - 0.29f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x - 0.3f), (int)(Position.y + 0.1f), (int)(Position.z + 0.29f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x - 0.3f), (int)(Position.y + 1.1f), (int)(Position.z - 0.29f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x - 0.3f), (int)(Position.y + 1.1f), (int)(Position.z + 0.29f) })->id != BLOCK_ID::Air;
	bool XCollisionPos = GameManager::Overworld->GetBlock({ (int)(Position.x + 0.3f), (int)(Position.y + 0.1f), (int)(Position.z - 0.29f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x + 0.3f), (int)(Position.y + 0.1f), (int)(Position.z + 0.29f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x + 0.3f), (int)(Position.y + 1.1f), (int)(Position.z - 0.29f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x + 0.3f), (int)(Position.y + 1.1f), (int)(Position.z + 0.29f) })->id != BLOCK_ID::Air;
	bool ZCollisionMinus = GameManager::Overworld->GetBlock({ (int)(Position.x - 0.29f), (int)(Position.y + 0.1f), (int)(Position.z - 0.3f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x + 0.29f), (int)(Position.y + 0.1f), (int)(Position.z - 0.3f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x - 0.29f), (int)(Position.y + 1.1f), (int)(Position.z - 0.3f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x + 0.29f), (int)(Position.y + 1.1f), (int)(Position.z - 0.3f) })->id != BLOCK_ID::Air;
	bool ZCollisionPos = GameManager::Overworld->GetBlock({ (int)(Position.x - 0.29f), (int)(Position.y + 0.1f), (int)(Position.z + 0.3f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x + 0.29f), (int)(Position.y + 0.1f), (int)(Position.z + 0.3f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x - 0.29f), (int)(Position.y + 1.1f), (int)(Position.z + 0.3f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x + 0.29f), (int)(Position.y + 1.1f), (int)(Position.z + 0.3f) })->id != BLOCK_ID::Air;
	bool YCollisionPos = GameManager::Overworld->GetBlock({ (int)(Position.x - 0.29f), (int)(Position.y + 1.9f), (int)(Position.z + 0.3f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x + 0.29f), (int)(Position.y + 1.9f), (int)(Position.z + 0.29f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x - 0.29f), (int)(Position.y + 1.9f), (int)(Position.z + 0.29f) })->id != BLOCK_ID::Air
		|| GameManager::Overworld->GetBlock({ (int)(Position.x + 0.29f), (int)(Position.y + 1.9f), (int)(Position.z + 0.29f) })->id != BLOCK_ID::Air;
	if (XCollisionPos && Velocity.x > 0) Velocity.x = 0;
	if (XCollisionMinus && Velocity.x < 0) Velocity.x = 0;
	if (ZCollisionPos && Velocity.z > 0) Velocity.z = 0;
	if (ZCollisionMinus && Velocity.z < 0) Velocity.z = 0;
	if (YCollisionPos && Velocity.y > 0) Velocity.y = 0;
	/*glm::vec3 vec(0.0f);
	glm::vec2 v(0.0f);
	if (Input::GetKeyState(GLFW_KEY_S) == GLFW_PRESS)
	{
		v.x -= cos(glm::radians(yaw));
		v.y -= sin(glm::radians(yaw));
	}
	if (Input::GetKeyState(GLFW_KEY_A) == GLFW_PRESS)
	{
		v.x -= cos(glm::radians(yaw + 90));
		v.y -= sin(glm::radians(yaw + 90));
	}
	if (Input::GetKeyState(GLFW_KEY_D) == GLFW_PRESS)
	{
		v.x += cos(glm::radians(yaw + 90));
		v.y += sin(glm::radians(yaw + 90));
	}
	if (v.x != 0 && v.y != 0)
		v = glm::normalize(v);
	vec.x = v.x;
	vec.z = v.y;
	vec *= speed * deltaTime;
	if (GameManager::Overworld->GetBlock({ (int)Position.x, (int)Position.y, (int)Position.z })->id != BLOCK_ID::Air && vec.y < 0)vec.y = 0.0f;
	Position.x += vec.x;
	Position.y += vec.y;
	Position.z += vec.z;*/
	Position += Velocity;
	mainCamera.cameraPos = glm::vec3(Position.x, Position.y + 1.8f, Position.z);
}