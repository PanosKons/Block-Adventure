#pragma once
#include "Common/InputAction.h"
namespace Script {
	class sInputAction
	{
	public:
		enum ActionName { LeftHandInteract, RightHandInteract, ReloadAssembly, OpenInventory, ActionNameSize };
		enum Button
		{
			LeftMouse = -3, RightMouse = -2, MiddleMouse = -1,
			Space = 32,
			Slash = 47,
			n0 = 48, n1, n2, n3, n4, n5, n6, n7, n8, n9,
			A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			Shift = 340, Control, Alt,
			EscapeKey = 256, Enter, Tab, BackSpace,
			RightArrow = 262, LeftArrow, DownArrow, UpArrow,
			F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
		};
		enum Screen
		{
			Game, ChatBox, GUI
		};
		static void Register(const InputAction&& ia);
		static void RegisterInputActions();
		static std::vector<InputAction>* GetInputActions() {
			return &inputActions;
		}
	private:
		inline static std::vector<InputAction> inputActions = std::vector<InputAction>(ActionNameSize);
	};
}
