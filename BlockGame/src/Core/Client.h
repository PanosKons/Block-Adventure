#pragma once
#include "pch.h"
struct GLFWwindow;
namespace Client {
	extern int ScreenWidth;
	extern int ScreenHeight;
	extern GLFWwindow* ApplicationWindow;
	extern GLFWwindow* ImGuiWindow;
	extern std::wstring ip;
	extern int port;
	extern bool ShouldStop;

	constexpr float charWidthOffset = 28.0f;
	constexpr float charHeightOffset = 32.0f;
	constexpr float charWidth = 32.0f;
	constexpr float charHeight = 32.0f;
};