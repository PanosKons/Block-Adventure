#pragma once
#include "../Math/Cuboid.h"
#include "../Math/Vector.h"
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "MainCamera.h"
#include "../Blocks/Block.h"
#include <array>
#include "ItemStack.h"
class Player
{
public:
	Player();
	Vector3<float> Position;
	Cuboid Hitbox;
	Block* GetFacingBlock();
	Block* GetBlockToPlace();
	void MarkBlockToBreak();
	void Update(float deltaTime);
	int GetFirstAvaiableSlot(BLOCK_ID id);
	float yaw = -90.0f;
	float pitch = -80.0f;
	float lastX = 0.0f;
	float lastY = 0.0f;
	float JumpCooldown = 0.0f;
	float speed = 5.0f;
	bool isBreakingBlock = false;
	float BlockPlaceDelay = 0.3f;
	float TimeToBreak = 0.0f;
	Block* breakingBlock = nullptr;
	int RenderDistance = 5;
	bool firstMouse = true;
	Vector3<float> Velocity;
	MainCamera mainCamera;
	std::array<ItemStack, 9> Inventory;
	char ActiveSlot;
	bool grounded;
	void CursorMoved(double xpos, double ypos);
};
