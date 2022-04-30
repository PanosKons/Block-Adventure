#pragma once
#include <Engine.h>
#include "Math/Vector.h"
#include "../Blocks.h"
constexpr int StructureSize = 8;
struct Structure
{
	Structure();
	std::array<BLOCK_ID, StructureSize* StructureSize* StructureSize> data;
	Vector3<int> Center;
};
