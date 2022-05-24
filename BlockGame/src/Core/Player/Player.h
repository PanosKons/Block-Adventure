#pragma once
#include "Math/Cuboid.h"
#include "Math/Vector.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "MainCamera.h"
#include "../Blocks/Block.h"
#include <Engine.h>
#include "ItemStack.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
constexpr int InventorySize = 9;
class Player
{
public:
	Player();
	~Player();
	void CursorMoved(double xpos, double ypos);
	Block GetFacingBlock();
	Block GetBlockToPlace();
	void MarkBlockToBreak();
	void Update(float deltaTime);
	void DrawPlayer(Block facingblock);
	int GetFirstAvaiableSlot(int id, TYPE type);
	Vector3<double> Hitbox;
	float yaw = -118.0f;
	float pitch = -43.0f;
	float lastX = 0.0f;
	float lastY = 0.0f;
	float JumpCooldown = 0.0f;
	float speed = 5.0f;
	float fov = 70.0f;
	float maxHealth = 100.0f;
	float health = 100.0f;
	bool isBreakingBlock = false;
	bool crouch = false;
	bool grounded = false;
	float BlockPlaceDelay = 0.3f;
	float TimeToBreak = 0.0f;
	Block breakingBlock;
	int RenderDistance = 5;
	bool firstMouse = true;
	Vector3<double> Velocity;
	MainCamera mainCamera;
	std::array<ItemStack, InventorySize> Inventory;
	char ActiveSlot;
	bool godmode = true;
	Vector3<double> Position;
	//Cached data
	Vector3<int> ChunkPosition = { 0,0,0 };

	//Rendering data
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
};
