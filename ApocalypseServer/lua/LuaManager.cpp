#include "include/lua.hpp"
#pragma comment(lib, "lua54.lib")
#include "LuaManager.h"
#include "Logger.h"
#include "Common/Blocks/Block.h"
#include "Common/Math/Noise.h"
static lua_State* L;
void Lua(int r, lua_State* L)
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
	Lua(luaL_dofile(L, "data.lua"),L);
	//GetBlockProperties
	{
	lua_getglobal(L, "Blocks");
	if (lua_istable(L, -1))
	{
		lua_pushnil(L);
		for (; lua_next(L, -2) != 0; lua_pop(L, 1)) {
			BlockProperties bp{};

			ASSERT(lua_isstring(L, -2), "Invalid lua script(blocks)");
			strcpy_s(bp.name.data(),bp.name.size() - 1, lua_tostring(L, -2));

			lua_getfield(L, -1, "Render");
			ASSERT(lua_isboolean(L, -1), "Invalid lua script(blocks)");
			bp.render = lua_toboolean(L, -1);
			lua_pop(L, 1);
			if (bp.render == false) goto end;

			lua_getfield(L, -1, "Transparent");
			ASSERT(lua_isboolean(L, -1), "Invalid lua script(blocks)");
			bp.transparent = lua_toboolean(L, -1);
			lua_pop(L, 1);

			lua_getfield(L, -1, "Texture");
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
	//GetNoise
	{
		lua_getglobal(L, "Noise");
		int Octaves;
		int YLevelStretch;
		double Frequency;
		int BiomeStretch;
		if (lua_istable(L, -1))
		{
			lua_pushstring(L, "Octaves");
			lua_gettable(L, -2);
			ASSERT(lua_isinteger(L, -1), "Invalid lua script(Noise)");
			Octaves = lua_tointeger(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "YLevelStretch");
			lua_gettable(L, -2);
			ASSERT(lua_isinteger(L, -1), "Invalid lua script(Noise)");
			YLevelStretch = lua_tointeger(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "Frequency");
			lua_gettable(L, -2);
			ASSERT(lua_isnumber(L, -1), "Invalid lua script(Noise)");
			Frequency = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "BiomeStretch");
			lua_gettable(L, -2);
			ASSERT(lua_isinteger(L, -1), "Invalid lua script(Noise)");
			BiomeStretch = lua_tointeger(L, -1);
			lua_pop(L, 1);
			Noise::SetNoiseSettings(Octaves, Frequency, YLevelStretch, BiomeStretch);
		}
	}
	//GetWorldGenerationProperties
	{
		lua_getglobal(L, "WorldGeneration");
		if (lua_istable(L, -1))
		{
			lua_getfield(L, -1, "Filler");
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

				ASSERT(lua_isstring(L, -2), "Invalid lua script(items)");
				strcpy_s(bp.name.data(), bp.name.size() - 1, lua_tostring(L, -2));

				lua_getfield(L, -1, "Texture");
				ASSERT(lua_isinteger(L, -1), "Invalid lua script(items)");
				bp.texture = (unsigned char)lua_tointeger(L, -1);
				lua_pop(L, 1);



				Item::itemProperties.push_back(bp);
			}
		}
	}
	//lua_close(L);
}

float LuaManager::OnBlockInteract(Player& player, Block block, BlockInteractState state)
{
	lua_getglobal(L, "OnBlockInteract");
	if (lua_isfunction(L, -1))
	{
		lua_pushstring(L, block.GetBlockProperties().name.data());
		if (player.Inventory[player.ActiveSlot].GetItemStackType() == ItemStackType::Item && player.Inventory[player.ActiveSlot].GetCount() > 0)
		{
			lua_pushstring(L, Item::GetItemProperties(player.Inventory[player.ActiveSlot].GetItemType()).name.data());
		}
		else
		{
			lua_pushnil(L);
		}
		lua_pcall(L, 2, 1, 0);
	}
	return lua_tonumber(L, -1);
}
