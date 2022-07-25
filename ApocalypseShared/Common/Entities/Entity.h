#pragma once
#include "Math/Vector.h"
class Entity
{
public:
	Entity();
	Vector3<double> Position;
	Vector3<double> Hitbox;
	Vector3<double> Velocity;
	float yaw = -118.0f;
	float pitch = -43.0f;
	float maxHealth = 100.0f;
	float health = 100.0f;
	float speed = 5.0f;
	bool grounded = false;
};

