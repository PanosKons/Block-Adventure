#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Input
{
public:
	static int GetKeyState(int key);
	static void SetCursorCallback(void (*CursorCallback)(GLFWwindow* window, double xpos, double ypos));
	static void SetMouseCallback(void (*MouseCallback)(GLFWwindow* window, int button, int action, int mods));
	static void SetKeyCallback(void (*KeyCallback)(GLFWwindow* window, int key, int actioncode, int action, int mods));
};
