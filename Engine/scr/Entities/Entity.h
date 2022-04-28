#pragma once
#include "../Math/Vector.h"
#include "../Math/Cuboid.h"
class Entity
{
	virtual void Update(float deltaTime);
	Vector3<float> Position, Velocity;
	Cuboid Hitbox;
	bool grounded;
};
