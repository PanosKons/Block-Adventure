#pragma once
#include "pch.h"
#include "Common/Blocks/Block.h"
#include "Common/Entities/Player/Player.h"

class LuaManager
{
public:
	static void LoadScripts();
	static void MouseEvent(uint64_t UUID, MouseState LeftMouse, MouseState RightMouse, MouseState MiddleMouse);
	static void UpdateEvent();
};

