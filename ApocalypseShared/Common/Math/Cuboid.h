#pragma once
#include "Vector.h"
class Cuboid
{
public:
	Vector3<float> Position;
	Vector3<float> Size;
	Cuboid(Vector3<float> Position, Vector3<float> Size);
};
