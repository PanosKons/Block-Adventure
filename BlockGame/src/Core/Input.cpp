#include "pch.h"
#include "Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Client.h"
void Input::SetCharCallback(void (*CharCallback)(unsigned int key))
{
	static auto WCharCallback = CharCallback;
	glfwSetCharCallback(Client::ApplicationWindow, [](GLFWwindow* window, unsigned int key) {WCharCallback(key); });
}
void Input::SetCursorCallback(void (*CursorCallback)(double xpos, double ypos))
{
	static auto WCursorCallback = CursorCallback;
	glfwSetCursorPosCallback(Client::ApplicationWindow, [](GLFWwindow* window, double Xpos, double Ypos) { WCursorCallback(Xpos, Ypos); });
}
void Input::SetMouseCallback(void (*MouseCallback)(int button, int action, int mods))
{
	static auto WMouseCallback = MouseCallback;
	glfwSetMouseButtonCallback(Client::ApplicationWindow, [](GLFWwindow* window, int button, int action, int mods) { WMouseCallback(button, action, mods); });
}
void Input::SetKeyCallback(void (*KeyCallback)(int key, int actioncode, int action, int mods))
{
	static auto WKeyCallback = KeyCallback;
	glfwSetKeyCallback(Client::ApplicationWindow, [](GLFWwindow* window, int key, int actioncode,int action, int mods) {WKeyCallback(key, actioncode,action, mods); });
}

int Input::GetKeyState(int key)
{
	return glfwGetKey(Client::ApplicationWindow, key);
}
int Input::GetMouseState(int Mouse)
{
	return glfwGetMouseButton(Client::ApplicationWindow, Mouse);
}