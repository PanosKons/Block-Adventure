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
	template<typename T>
	static std::string ToString(T Number)
	{
		std::ostringstream os;

		os << Number;
		return os.str();
	}
	static void PrintString(std::string&& Text, Vector2<float> position);
	static void PrintString(std::string& Text, Vector2<float> position);
	static void PrintSquare(Vector2<float> Position, Vector2<float> Size, Vector4<float> Color, float TextureID);
};
