#pragma once
#include "Common/Math/Vector.h"
enum Mouse
{
	Left, Right, Middle
};
enum Key
{
	Space = 32,
	Slash = 47,
	n0 = 48, n1, n2, n3, n4, n5, n6, n7, n8, n9,
	A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	Shift = 340, Control, Alt,
	EscapeKey = 256, Enter, Tab, BackSpace,
	RightArrow = 262, LeftArrow, DownArrow, UpArrow,
	F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
};
enum ButtonState
{
	Click, Hold, Release
};
class Input
{
public:
	static void SetCursorCallback(void (*CursorCallback)(double xpos, double ypos));
	static void SetMouseCallback(void (*MouseCallback)(int button, int action, int mods));
	static void SetKeyCallback(void (*KeyCallback)(int key, int actioncode, int action, int mods));
	static void SetCharCallback(void (*CharCallback)(unsigned int key));

	static ButtonState GetKeyState(Key key);
	static ButtonState GetMouseState(Mouse Mouse);
	static Vector2<double> GetCursorPosition();
};
