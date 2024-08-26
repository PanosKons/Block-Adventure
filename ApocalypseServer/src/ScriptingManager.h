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

	static void OnCommandEvent(uint64_t UUID, Command& command);
	static void OnActionEvent(uint64_t UUID, int identifier);
	static void GlobalUpdateEvent(double TimeStep);
	static void OnGlobalUpdateEvent(double TimeStep);
	static void OnPlayerUpdateEvent(uint64_t UUID);
};

