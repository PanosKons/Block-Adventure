#pragma once
#include "../Math/Cuboid.h"
#include "../Math/Vector.h"
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "MainCamera.h"
#include "Block.h"
class Player
{
public:
	Player();
	Vector3<float> Position;
	Cuboid Hitbox;
	Block* GetFacingBlock();
	Block* GetBlockToPlace();
	void Update(float deltaTime);
	float yaw = -90.0f;
	float pitch = -80.0f;
	float lastX = 0.0f;
	float lastY = 0.0f;
	bool firstMouse = true;
	float speed = 10;
	Vector3<float> Velocity;
	MainCamera mainCamera;
	bool grounded;
	void MouseButton(int button, int action);
	void CursorMoved(double xpos, double ypos);
};
