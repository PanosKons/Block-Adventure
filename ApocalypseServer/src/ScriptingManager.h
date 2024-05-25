#pragma once
#include "pch.h"
#include "Common/Entities/Player/Player.h"

class ScriptingManager
{
public:
	static void Load();
	static void Start();
	static void Shutdown();

	static void ReloadAssembly();

	static void RegisterInternalCalls();

	static void OnMouseEvent(uint64_t UUID, MouseState LeftMouse, MouseState RightMouse, MouseState MiddleMouse);
	static void OnCommandEvent(uint64_t UUID, Command& command);
	static void OnKeyEvent(uint64_t UUID, bool PKeyPressed, bool RKeyPressed, bool EKeyPressed);
	static void GlobalUpdateEvent(double TimeStep);
	static void OnGlobalUpdateEvent(double TimeStep);
	static void OnPlayerUpdateEvent(uint64_t UUID);
};

