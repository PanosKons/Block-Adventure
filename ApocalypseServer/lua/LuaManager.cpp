#include "include/lua.hpp"
#pragma comment(lib, "lua54.lib")
#include "LuaManager.h"
#include "Logger.h"
static lua_State* L;
void Lua(int r)
{
	if (r != LUA_OK)
	{
		std::string msg = lua_tostring(L, -1);
		ERR(msg);
	}
}
enum class Tooltype
{
	Pickaxe, Shovel, Axe, None
};
Tooltype Totooltype(std::string in)
{
	if (in == "pickaxe")
	{
		return Tooltype::Pickaxe;
	}
	if (in == "shovel")
	{
		return Tooltype::Shovel;
	}
	if (in == "none")
	{
		return Tooltype::None;
	}
	if (in == "axe")
	{
		return Tooltype::Axe;
	}
	ERR("INVALID TOOL TYPE");
	return Tooltype::None;
}
struct Tool {
	Tooltype type;
	unsigned int minimumLevel;
};
struct BlockPropertiesLua
{
	std::string name;
	unsigned int hardness;
	Tool tool;
	std::array<unsigned char, 6> textureSides;
	bool render;
};
static std::vector<BlockPropertiesLua> bps;
void LuaManager::Start()
{
	L = luaL_newstate();
	Lua(luaL_dofile(L, "script.lua"));
	//GetBlockProperties
	{
	lua_getglobal(L, "Blocks");
	if (lua_istable(L, -1))
	{
		lua_pushnil(L);
		for (; lua_next(L, -2) != 0; lua_pop(L, 1)) {
			BlockPropertiesLua bp;

			lua_pushstring(L, "Id");
			lua_gettable(L, -2);
			ASSERT(lua_isstring(L, -1), "Invalid lua script(blocks)");
			bp.name = lua_tostring(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "Render");
			lua_gettable(L, -2);
			ASSERT(lua_isboolean(L, -1), "Invalid lua script(blocks)");
			bp.render = lua_toboolean(L, -1);
			lua_pop(L, 1);
			if (bp.render == false) goto end;

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
			bp.tool.type = Totooltype(lua_tostring(L, -1));
			lua_pop(L, 1);
			lua_pushstring(L, "MinimumLevel");
			lua_gettable(L, -2);
			ASSERT(lua_isinteger(L, -1), "Invalid lua script(blocks)");
			bp.tool.minimumLevel = (unsigned int)lua_tointeger(L, -1);
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
			bps.push_back(bp);
		}
	}
	}
	//GetWorldGenerationProperties
	{

	}
	lua_close(L);
}
