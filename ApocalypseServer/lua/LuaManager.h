#pragma once
#include "pch.h"
#include "Common/Blocks/Block.h"

class LuaManager
{
public:
	static void LoadScripts();
	static BlockType GetBlockType(std::string&& key);
};

