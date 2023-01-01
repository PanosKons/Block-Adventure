#pragma once
#include "pch.h"
#include "Common/Math/Vector.h"
#include "Common/Blocks/Block.h"

constexpr int StructureSize = 8;
struct Structure
{
	Structure();
	std::array<BlockType, StructureSize* StructureSize* StructureSize> data;
	Vector3<int> Center;
};
