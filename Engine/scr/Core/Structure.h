#pragma once
#include <Engine.h>
#include "Math/Vector.h"
#include "Blocks.h"
constexpr int StructureSize = 32;
struct Structure
{
	Structure(Vector3<int> Center, const char* path);
	Structure(Vector3<int> Center);
	std::array<BLOCK_ID, StructureSize* StructureSize* StructureSize>* data;
	Vector3<int> Center;
};
