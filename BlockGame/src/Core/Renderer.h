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
	void HideCursor(bool value);
};
namespace RenderBuilder
{
	void Begin(RenderData& renderData);
	void AddSquare(RenderData& renderData, Vector2<float> Position, Vector2<float> Size, Vector4<float> Color, Vector2<float> TexCords, Vector2<float> TexSize, float TextureID, float layer = 0.0f);
	void AddText(RenderData& renderData, std::string_view Text, Vector2<float> Position, float layer = 0.0f);
	void AddCube(RenderData& renderData, Vector3<float> Position, Vector3<float> Size, Vector4<float> Color);
	void End(RenderData& renderData);
};