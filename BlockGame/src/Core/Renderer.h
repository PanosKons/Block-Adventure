#pragma once
#include "pch.h"
#define NOMINMAX
#include "Rendering/RenderData.h"
#include "Common/Math/Vector.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"


namespace Renderer
{
	enum View
	{
		None, Player, UI
	};
	struct RenderCommand
	{
		RenderData* renderData;
		View view;
		bool Depth;
	};
	int CreateWindow(const std::string& name);
	void Render();
	void ShutDown();
	bool ShouldWindowClose();
	void AddCommand(RenderCommand renderCommand);
};
