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
class Player
{
public:
	Player();
	~Player();
	Cuboid Hitbox;
	Block* GetFacingBlock();
	Block* GetBlockToPlace();
	void MarkBlockToBreak();
	void Update(float deltaTime);
	void DrawPlayer(Block* facingblock);
	int GetFirstAvaiableSlot(int id, TYPE type);
	float yaw = -118.0f;
	float pitch = -43.0f;
	float lastX = 0.0f;
	float lastY = 0.0f;
	float JumpCooldown = 0.0f;
	float speed = 5.0f;
	float fov = 70.0f;
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
	bool godmode = false;
	void CursorMoved(double xpos, double ypos);
	//Cached data
	Vector3<int> ChunkPosition = { 0,0,0 };

	Vector3<float> Position;

	//Rendering data
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
};
