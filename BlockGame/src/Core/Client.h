#pragma once
#include "pch.h"
struct GLFWwindow;
namespace Client {
	extern int ScreenWidth;
	extern int ScreenHeight;
	extern GLFWwindow* ApplicationWindow;
	extern int FPS;
	extern bool Playing;
	extern std::wstring ip;
	extern int port;
	extern bool ShuttingDown;
};