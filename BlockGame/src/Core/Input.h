#pragma once
#include "Common/Math/Vector.h"
enum MouseState;
class Input
{
public:
	static void SetCursorCallback(void (*CursorCallback)(double xpos, double ypos));
	static void SetMouseCallback(void (*MouseCallback)(int button, int action, int mods));
	static void SetKeyCallback(void (*KeyCallback)(int key, int actioncode, int action, int mods));
	static void SetCharCallback(void (*CharCallback)(unsigned int key));

	static int GetKeyState(int key);
	static MouseState GetMouseState(int Mouse);
	static Vector2<double> GetCursorPosition();
};
