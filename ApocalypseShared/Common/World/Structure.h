#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"
#include "Common/Blocks/Block.h"

constexpr int StructureSize = 8;
struct Structure
{
	Structure();
	std::array<BLOCK_ID, StructureSize* StructureSize* StructureSize> data;
	Vector3<int> Center;
};
