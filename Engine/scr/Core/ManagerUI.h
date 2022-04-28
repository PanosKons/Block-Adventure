#pragma once
#include <memory>
#include <string>
#include "../Rendering/IndexBuffer.h"
#include "../Rendering/VertexBuffer.h"
class ManagerUI
{
public:
	static void Init();
	static void UpdateUI();
	static std::string ToString(bool value);
	static void PrintString(std::string&& Text, Vector3<float> position);
	static void PrintString(std::string& Text, Vector3<float> position);
	static void PrintSquare(Vector3<float> Position, Vector2<float> Size, Vector4<float> Color, Vector2<float> TexCords, Vector2<float> TexSize, float TextureID);
	static void PrintSquare(Vector3<float> Position, Vector2<float> Size, Vector4<float> Color, float TextureID);
};
