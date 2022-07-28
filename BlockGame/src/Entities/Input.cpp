#include "pch.h"
#include "Input.h"
#include "Renderer.h"
#include "Client.h"

void Input::SetCharCallback(void (*CharCallback)(GLFWwindow* window, unsigned int key))
{
	glfwSetCharCallback(Client::ApplicationWindow, CharCallback);
}
void Input::SetCursorCallback(void (*CursorCallback)(GLFWwindow* window, double xpos, double ypos))
{
	glfwSetCursorPosCallback(Client::ApplicationWindow, CursorCallback);
}
void Input::SetMouseCallback(void (*MouseCallback)(GLFWwindow* window, int button, int action, int mods))
{
	glfwSetMouseButtonCallback(Client::ApplicationWindow, MouseCallback);
}
void Input::SetKeyCallback(void (*KeyCallback)(GLFWwindow* window, int key, int actioncode, int action, int mods))
{
	glfwSetKeyCallback(Client::ApplicationWindow, KeyCallback);
}
int Input::GetKeyState(int key)
{
	return glfwGetKey(Client::ApplicationWindow, key);
}
int Input::GetMouseState(int Mouse)
{
	return glfwGetMouseButton(Client::ApplicationWindow, Mouse);
}