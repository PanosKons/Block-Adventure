#pragma once
#include "pch.h"
#include "Common/Entities/Player/Player.h"

class ScriptingManager
{
public:
	static void Load();
	static void Shutdown();

	static void RegisterInternalCalls();
	static void OnMouseEvent(uint64_t UUID, MouseState LeftMouse, MouseState RightMouse, MouseState MiddleMouse);
	static void OnCommandEvent(uint64_t UUID, Command& command);
	static void UpdateEvent();
};

