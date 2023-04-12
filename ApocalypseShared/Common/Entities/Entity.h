#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"

typedef short EntityType;

class Entity
{
public:
	Entity();
	uint64_t UUID;
	Vector3<double> Position;
	Vector3<double> Hitbox;
	EntityType entityType;
	//Vector3<double> Velocity;
	//float Yaw;
	//float Pitch;
	//float MaxHealth;
	//float Health;
	//float Speed;
	//bool Grounded;
};

