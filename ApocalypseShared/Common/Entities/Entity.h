#pragma once
#include "Math/Vector.h"
class Entity
{
public:
	Entity();
	Vector3<double> Position;
	Vector3<double> Hitbox;
	Vector3<double> Velocity;
	float yaw;
	float pitch;
	float maxHealth;
	float health;
	float speed;
	bool grounded;
};

