#include "include/lua.hpp"
#pragma comment(lib, "lua54.lib")
#include "LuaManager.h"
#include "Logger.h"
#include "Common/Blocks/Block.h"

static lua_State* L;
void Lua(int r)
{
	if (r != LUA_OK)
	{
		std::string msg = lua_tostring(L, -1);
		ERR(msg);
	}
}
void LuaManager::LoadScripts()
{
	L = luaL_newstate();
	Lua(luaL_dofile(L, "script.lua"));
	//GetTooltypes
	{
		lua_getglobal(L, "ToolTypes");
		if (lua_istable(L, -1))
		{
			lua_pushnil(L);
			for (; lua_next(L, -2) != 0; lua_pop(L, 1))
			{
				ASSERT(lua_isstring(L, -1), "Invalid lua script(ToolTypes)");
				Block::toolTypes.push_back(std::array<char,MaxIdLength>());
				strcpy_s(Block::toolTypes.back().data(), Block::toolTypes.back().size() - 1, lua_tostring(L, -1));
			}
		}
	}
	//GetBlockProperties
	{
	lua_getglobal(L, "Blocks");
	if (lua_istable(L, -1))
	{
		lua_pushnil(L);
		for (; lua_next(L, -2) != 0; lua_pop(L, 1)) {
			BlockProperties bp{};

			lua_pushstring(L, "Id");
			lua_gettable(L, -2);
			ASSERT(lua_isstring(L, -1), "Invalid lua script(blocks)");
			strcpy_s(bp.name.data(),bp.name.size() - 1, lua_tostring(L, -1));
			lua_pop(L, 1);

			lua_pushstring(L, "Render");
			lua_gettable(L, -2);
			ASSERT(lua_isboolean(L, -1), "Invalid lua script(blocks)");
			bp.render = lua_toboolean(L, -1);
			lua_pop(L, 1);
			if (bp.render == false) goto end;

			lua_pushstring(L, "Transparent");
			lua_gettable(L, -2);
			ASSERT(lua_isboolean(L, -1), "Invalid lua script(blocks)");
			bp.transparent = lua_toboolean(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "Hardness");
			lua_gettable(L, -2);
			ASSERT(lua_isinteger(L, -1), "Invalid lua script(blocks)");
			bp.hardness = (unsigned int)lua_tointeger(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "Tool");
			lua_gettable(L, -2);
			lua_pushstring(L, "Id");
			lua_gettable(L, -2);
			ASSERT(lua_isstring(L, -1), "Invalid lua script(blocks)");
			std::array<char, MaxIdLength> str;
			strcpy_s(str.data(), str.size() - 1, lua_tostring(L, -1));
			bp.tool.ToolId = (int)(std::find(Block::toolTypes.begin(), Block::toolTypes.end(),str) - Block::toolTypes.begin());
			lua_pop(L, 1);
			lua_pushstring(L, "MinimumLevel");
			lua_gettable(L, -2);
			ASSERT(lua_isinteger(L, -1), "Invalid lua script(blocks)");
			bp.tool.ToolMinimumLevel = (unsigned int)lua_tointeger(L, -1);
			lua_pop(L, 2);

			lua_pushstring(L, "Texture");
			lua_gettable(L, -2);
			lua_pushnil(L);
			for (size_t i = 0; i < bp.textureSides.size(); i++)
			{
				lua_next(L, -2);
				bp.textureSides[i] = (unsigned char)lua_tointeger(L, -1);
				lua_pop(L, 1);
			}
			bp.render = true;
			lua_pop(L, 2);
		end:
			Block::blockProperties.push_back(bp);
		}
	}
	}
	//GetWorldGenerationProperties
	{
		lua_getglobal(L, "WorldGeneration");
		if (lua_istable(L, -1))
		{
			lua_pushstring(L, "Filler");
			lua_gettable(L, -2);
			ASSERT(lua_isstring(L, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::FillerBlock = Block::GetBlockType(lua_tostring(L,-1));
			lua_pop(L, 1);

			lua_pushstring(L, "Underground");
			lua_gettable(L, -2);
			ASSERT(lua_isstring(L, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::UndergroundBlock = Block::GetBlockType(lua_tostring(L, -1));
			lua_pop(L, 1);

			lua_pushstring(L, "Dirt");
			lua_gettable(L, -2);
			ASSERT(lua_isstring(L, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::DirtBlock = Block::GetBlockType(lua_tostring(L, -1));
			lua_pop(L, 1);

			lua_pushstring(L, "DryTop");
			lua_gettable(L, -2);
			ASSERT(lua_isstring(L, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::DryTopBlock = Block::GetBlockType(lua_tostring(L, -1));
			lua_pop(L, 1);

			lua_pushstring(L, "WetTop");
			lua_gettable(L, -2);
			ASSERT(lua_isstring(L, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::WetTopBlock = Block::GetBlockType(lua_tostring(L, -1));
			lua_pop(L, 1);

			lua_pushstring(L, "DeadTop");
			lua_gettable(L, -2);
			ASSERT(lua_isstring(L, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::DeadTopBlock = Block::GetBlockType(lua_tostring(L, -1));
			lua_pop(L, 1);

			lua_pushstring(L, "StoneTop");
			lua_gettable(L, -2);
			ASSERT(lua_isstring(L, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::StoneTopBlock = Block::GetBlockType(lua_tostring(L, -1));
			lua_pop(L, 1);

			lua_pushstring(L, "Ore");
			lua_gettable(L, -2);
			ASSERT(lua_isstring(L, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::OreBlock = Block::GetBlockType(lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
	//GetItemProperties
	{
		lua_getglobal(L, "Items");
		if (lua_istable(L, -1))
		{
			lua_pushnil(L);
			for (; lua_next(L, -2) != 0; lua_pop(L, 1)) {
				ItemProperties bp{};

				lua_pushstring(L, "Id");
				lua_gettable(L, -2);
				ASSERT(lua_isstring(L, -1), "Invalid lua script(items)");
				strcpy_s(bp.name.data(), bp.name.size() - 1, lua_tostring(L, -1));
				lua_pop(L, 1);

				lua_pushstring(L, "Texture");
				lua_gettable(L, -2);
				ASSERT(lua_isinteger(L, -1), "Invalid lua script(items)");
				bp.texture = (unsigned char)lua_tointeger(L, -1);
				lua_pop(L, 1);
				Item::itemProperties.push_back(bp);
			}
		}
	}
	lua_close(L);
}
