#include "Input.h"
#include "Renderer.h"
#include "GlobalVariables.h"
void Input::SetCursorCallback(void (*CursorCallback)(GLFWwindow* window, double xpos, double ypos))
{
	glfwSetCursorPosCallback(ApplicationWindow, CursorCallback);
}
void Input::SetMouseCallback(void (*MouseCallback)(GLFWwindow* window, int button, int action, int mods))
{
	glfwSetMouseButtonCallback(ApplicationWindow, MouseCallback);
}
void Input::SetKeyCallback(void (*KeyCallback)(GLFWwindow* window, int key, int actioncode, int action, int mods))
{
	glfwSetKeyCallback(ApplicationWindow, KeyCallback);
}
int Input::GetKeyState(int key)
{
	return glfwGetKey(ApplicationWindow, key);
}