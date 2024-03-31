#pragma once
#include "pch.h"
#define NOMINMAX
#include "Rendering/RenderData.h"
#include "Common/Math/Vector.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Client.h"
#undef CreateWindow

class Renderer
{
public:
	enum class RenderCommandType
	{
		World, UI2D, UI3D
	};
	struct RenderCommand
	{
		RenderData* renderData;
		RenderCommandType renderCommandType;
	};
	static void Init();
	static GLFWwindow* ConstructWindow(const std::string& name, bool resizable);
	static void Setup();
	static void SetupImGui();
	static void Render();
	static void ShutDown();
	static bool ShouldWindowClose(GLFWwindow* window);
	static void AddCommand(RenderCommand& renderCommand);
	static void HideCursor(bool value);
	static void BindWindow(GLFWwindow* window);
private:
};