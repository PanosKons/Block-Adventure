#pragma once
#include "pch.h"
#include "Math/Vector.h"
class Entity
{
public:
	Entity();
	Vector3<double> Position;
	Vector3<double> Hitbox;
	Vector3<double> Velocity;
	float Yaw;
	float Pitch;
	float MaxHealth;
	float Health;
	float Speed;
	bool Grounded;
};

