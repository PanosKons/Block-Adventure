#pragma once
#include "pch.h"
#include "Common/Blocks/Block.h"
#include "Common/Entities/Player/Player.h"

class LuaManager
{
public:
	static void LoadScripts();
	static float OnBlockInteract(Player& player,Block block,BlockInteractState state);
};

