#pragma once
#include "pch.h"
#include "Rendering/RenderData.h"
class MeshParser
{
public:
	static void ParseMesh(std::string&& path, RenderData& renderData);
};

