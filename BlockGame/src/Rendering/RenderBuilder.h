#pragma once
#include "Rendering/RenderData.h"

class RenderBuilder
{
public:
	static void Begin(RenderData& renderData);
	static void AddSquare(RenderData& renderData, Vector2<float> Position, Vector2<float> Size, Vector4<float> Color, Vector2<float> TexCords, Vector2<float> TexSize, float TextureID, float layer = 0.0f);
	static void AddText(RenderData& renderData, std::string_view Text, Vector2<float> Position, float layer = 0.0f);
	static void AddCube(RenderData& renderData, Vector3<float> Position, Vector3<float> Size, Vector4<float> Color);
	static void AddMesh(RenderData& renderData, Vector3<float> Position, Vector3<float> Size, Vector4<float> Color, unsigned int slot);
	static void End(RenderData& renderData);
};