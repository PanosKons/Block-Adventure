#pragma once
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
	EscapeKey = 256, Enter, Tab, BackSpace
};
enum Action
{
	Release, Press, Repeat
};
class Input
{
public:
	static void SetCursorCallback(void (*CursorCallback)(double xpos, double ypos));
	static void SetMouseCallback(void (*MouseCallback)(int button, int action, int mods));
	static void SetKeyCallback(void (*KeyCallback)(int key, int actioncode, int action, int mods));
	static void SetCharCallback(void (*CharCallback)(unsigned int key));

	static int GetKeyState(int key);
	static int GetMouseState(int Mouse);
};
