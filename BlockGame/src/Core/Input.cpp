#include "pch.h"
#include "Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Client.h"
#include "Common/Entities/Player/Player.h"

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

ButtonState Input::GetKeyState(Key key)
{
	static std::array<bool, 348> HoldState;
	ButtonState ReturnState = ButtonState::Release;
	int action = glfwGetKey(Client::ApplicationWindow, key);
	if (action == GLFW_RELEASE)
		HoldState[key] = false;
	if (action == GLFW_PRESS)
	{
		if (HoldState[key] == false)
			ReturnState = ButtonState::Click;
		else
			ReturnState = ButtonState::Hold;
		HoldState[key] = true;
	}
	return ReturnState;
}
ButtonState Input::GetMouseState(Mouse mouse)
{
	static std::array<bool, 3> HoldState;
	ButtonState ReturnState = ButtonState::Release;
	int action = glfwGetMouseButton(Client::ApplicationWindow, mouse);
	if (action == GLFW_RELEASE)
		HoldState[mouse] = false;
	if (action == GLFW_PRESS)
	{
		if (HoldState[mouse] == false)
			ReturnState = ButtonState::Click;
		else
			ReturnState = ButtonState::Hold;
		HoldState[mouse] = true;
	}
	return ReturnState;
}
Vector2<double> Input::GetCursorPosition()
{
	Vector2<double> ret;
	glfwGetCursorPos(Client::ApplicationWindow, &ret.x, &ret.y);
	ret.y = Client::ScreenHeight - ret.y;
	return ret;
}
