#include "include/lua.hpp"
#pragma comment(lib, "lua54.lib")
#include "LuaManager.h"
#include "Logger.h"
#include "Common/Blocks/Block.h"
#include "Common/Math/Noise.h"
#include "EntityManagerServer.h"
#include "NetworkingServer.h"
#include "World/WorldManagerServer.h"

static lua_State* l;
void Lua(int r, lua_State* L)
{
	if (r != LUA_OK)
	{
		std::string msg = lua_tostring(L, -1);
		ERR(msg);
	}
}
int Lua_Info(lua_State* l)
{
	INFO(lua_tostring(l,-1));
	return 0;
}
int Lua_GetPlayerName(lua_State* L)
{
	uint64_t UUID = lua_tointeger(L,-1);
	lua_pushstring(L,EntityManagerServer::GetPlayer(UUID)->credentials.Name.data());
	return 1;
}
int Lua_GetPlayerPosition(lua_State* L)
{
	uint64_t UUID = lua_tointeger(L, -1);
	Vector3<double>* Position = (Vector3<double>*)lua_newuserdata(L, sizeof(Vector3<double>));
	*Position = EntityManagerServer::GetPlayer(UUID)->Position;
	luaL_getmetatable(l, "DoubleVector3MetaTable");
	lua_setmetatable(l, -2);
	return 1;
}
int Lua_Vector3Double(lua_State* L)
{
	double x = lua_tonumber(L, -3);
	double y = lua_tonumber(L, -2);
	double z = lua_tonumber(L, -1);
	Vector3<double>* vec = (Vector3<double>*)lua_newuserdata(L, sizeof(Vector3<double>));
	*vec = { x,y,z };
	return 1;
}
int Lua_SetPlayerPosition(lua_State* L)
{
	uint64_t UUID = lua_tointeger(L, -2);
	Vector3<double>* vec = (Vector3<double>*)lua_touserdata(L, -1);

	EntityManagerServer::GetPlayer(UUID)->Position = *vec;
	NetworkingServer::SendPlayerPositionPacket(UUID);
	return 0;
}
int Lua_GetPlayerFacingBlock(lua_State* L)
{
	uint64_t UUID = lua_tointeger(L, -1);
	Block* block = (Block*)lua_newuserdata(L, sizeof(Block));
	*block = EntityManagerServer::GetPlayer(UUID)->GetFacingBlock();
	luaL_getmetatable(l, "BlockMetaTable");
	lua_setmetatable(l, -2);
	return 1;
}
int Lua_ReplaceBlock(lua_State* L)
{
	Block* block = (Block*)lua_touserdata(L, -2);
	std::string newblock = lua_tostring(L, -1);
	BlockType newtype = Block::GetBlockType(newblock);
	WorldManagerServer::ReplaceBlock(*block, newtype);
	NetworkingServer::SendReplaceBlockPacket(*block, newtype);
	return 0;
}
struct Lua_Block
{
	static int GetPosition(lua_State* L)
	{
		Block* block = (Block*)lua_touserdata(L, -1);
		Vector3<int>* vec = (Vector3<int>*)lua_newuserdata(l, sizeof(Vector3<int>));
		*vec = block->Position;
		luaL_getmetatable(l, "IntVector3MetaTable");
		lua_setmetatable(l, -2);
		return 1;
	}
	static int IsValid(lua_State* L)
	{
		Block* block = (Block*)lua_touserdata(L, -1);
		lua_pushboolean(L, block->IsValid());
		return 1;
	}
};
struct IntVector3
{
	static int Create(lua_State* l)
	{
		Vector3<int>* vec = (Vector3<int>*)lua_newuserdata(l, sizeof(Vector3<int>));
		*vec = { 0,0,0 };
		luaL_getmetatable(l, "IntVector3MetaTable");
		lua_setmetatable(l, -2);
		return 1;
	}
	static int Set(lua_State* l)
	{
		int x = lua_tointeger(l, -3);
		int y = lua_tointeger(l, -2);
		int z = lua_tointeger(l, -1);
		Vector3<int>* vec = (Vector3<int>*)lua_newuserdata(l, sizeof(Vector3<int>));
		*vec = { x,y,z };

		return 1;
	}
	static int GetX(lua_State* l)
	{
		Vector3<int>* vec = (Vector3<int>*)lua_touserdata(l, -1);
		lua_pushinteger(l, vec->x);
		return 1;
	}
	static int GetY(lua_State* l)
	{
		Vector3<int>* vec = (Vector3<int>*)lua_touserdata(l, -1);
		lua_pushinteger(l, vec->y);
		return 1;
	}
	static int GetZ(lua_State* l)
	{
		Vector3<int>* vec = (Vector3<int>*)lua_touserdata(l, -1);
		lua_pushinteger(l, vec->z);
		return 1;
	}
};
struct DoubleVector3
{
	static int Create(lua_State* l)
	{
		Vector3<double>* vec = (Vector3<double>*)lua_newuserdata(l, sizeof(Vector3<double>));
		*vec = { 0,0,0 };
		luaL_getmetatable(l, "DoubleVector3MetaTable");
		lua_setmetatable(l, -2);
		return 1;
	}
	static int Set(lua_State* l)
	{
		int x = lua_tonumber(l, -3);
		int y = lua_tonumber(l, -2);
		int z = lua_tonumber(l, -1);
		Vector3<double>* vec = (Vector3<double>*)lua_newuserdata(l, sizeof(Vector3<double>));
		*vec = Vector3<double>( x,y,z );

		return 1;
	}
	static int GetX(lua_State* l)
	{
		Vector3<double>* vec = (Vector3<double>*)lua_touserdata(l, -1);
		lua_pushnumber(l, vec->x);
		return 1;
	}
	static int GetY(lua_State* l)
	{
		Vector3<double>* vec = (Vector3<double>*)lua_touserdata(l, -1);
		lua_pushnumber(l, vec->y);
		return 1;
	}
	static int GetZ(lua_State* l)
	{
		Vector3<double>* vec = (Vector3<double>*)lua_touserdata(l, -1);
		lua_pushnumber(l, vec->z);
		return 1;
	}
};
void LuaManager::LoadScripts()
{
	l = luaL_newstate();
	Lua(luaL_dofile(l, "data.lua"),l);

	//Push functions
	lua_pushcfunction(l, Lua_Info);
	lua_setglobal(l, "INFO");
	lua_pushcfunction(l, Lua_GetPlayerName);
	lua_setglobal(l, "GetPlayerName");
	lua_pushcfunction(l, Lua_SetPlayerPosition);
	lua_setglobal(l, "SetPlayerPosition");
	lua_pushcfunction(l, Lua_GetPlayerPosition);
	lua_setglobal(l, "GetPlayerPosition");
	lua_pushcfunction(l, Lua_GetPlayerFacingBlock);
	lua_setglobal(l, "GetPlayerFacingBlock");
	lua_pushcfunction(l, Lua_ReplaceBlock);
	lua_setglobal(l, "ReplaceBlock");

	lua_newtable(l);
	int IntVector3Table = lua_gettop(l);
	lua_pushvalue(l, IntVector3Table);
	lua_setglobal(l, "IntVector3");

	lua_pushcfunction(l, IntVector3::Create);
	lua_setfield(l, -2, "Create");

	lua_pushcfunction(l, IntVector3::Set);
	lua_setfield(l, -2, "Set");

	lua_pushcfunction(l, IntVector3::GetX);
	lua_setfield(l, -2, "GetX");

	lua_pushcfunction(l, IntVector3::GetY);
	lua_setfield(l, -2, "GetY");

	lua_pushcfunction(l, IntVector3::GetZ);
	lua_setfield(l, -2, "GetZ");

	luaL_newmetatable(l, "IntVector3MetaTable");
	lua_pushvalue(l, IntVector3Table);
	lua_setfield(l, -2, "__index");

	lua_newtable(l);
	int DoubleVector3Table = lua_gettop(l);
	lua_pushvalue(l, DoubleVector3Table);
	lua_setglobal(l, "DoubleVector3");

	lua_pushcfunction(l, DoubleVector3::Create);
	lua_setfield(l, -2, "Create");

	lua_pushcfunction(l, DoubleVector3::Set);
	lua_setfield(l, -2, "Set");

	lua_pushcfunction(l, DoubleVector3::GetX);
	lua_setfield(l, -2, "GetX");

	lua_pushcfunction(l, DoubleVector3::GetY);
	lua_setfield(l, -2, "GetY");

	lua_pushcfunction(l, DoubleVector3::GetZ);
	lua_setfield(l, -2, "GetZ");


	luaL_newmetatable(l, "DoubleVector3MetaTable");
	lua_pushvalue(l, DoubleVector3Table);
	lua_setfield(l, -2, "__index");

	lua_newtable(l);
	int BlockTable = lua_gettop(l);
	lua_pushvalue(l, BlockTable);
	lua_setglobal(l, "Block");

	lua_pushcfunction(l, Lua_Block::GetPosition);
	lua_setfield(l, -2, "GetPosition");
	lua_pushcfunction(l, Lua_Block::IsValid);
	lua_setfield(l, -2, "IsValid");

	luaL_newmetatable(l, "BlockMetaTable");
	lua_pushvalue(l, BlockTable);
	lua_setfield(l, -2, "__index");

	//GetBlockProperties
	{
	lua_getglobal(l, "Blocks");
	if (lua_istable(l, -1))
	{
		lua_pushnil(l);
		for (; lua_next(l, -2) != 0; lua_pop(l, 1)) {
			BlockProperties bp{};

			ASSERT(lua_isstring(l, -2), "Invalid lua script(blocks)");
			strcpy_s(bp.name.data(),bp.name.size() - 1, lua_tostring(l, -2));

			lua_getfield(l, -1, "Render");
			ASSERT(lua_isboolean(l, -1), "Invalid lua script(blocks)");
			bp.render = lua_toboolean(l, -1);
			lua_pop(l, 1);
			if (bp.render == false) goto end;

			lua_getfield(l, -1, "Transparent");
			ASSERT(lua_isboolean(l, -1), "Invalid lua script(blocks)");
			bp.transparent = lua_toboolean(l, -1);
			lua_pop(l, 1);

			lua_getfield(l, -1, "Texture");
			lua_pushnil(l);
			for (size_t i = 0; i < bp.textureSides.size(); i++)
			{
				lua_next(l, -2);
				bp.textureSides[i] = (unsigned char)lua_tointeger(l, -1);
				lua_pop(l, 1);
			}
			bp.render = true;
			lua_pop(l, 2);
		end:
			Block::blockProperties.push_back(bp);
		}
	}
	}
	//GetNoise
	{
		lua_getglobal(l, "Noise");
		int Octaves;
		int YLevelStretch;
		double Frequency;
		int BiomeStretch;
		if (lua_istable(l, -1))
		{
			lua_getfield(l, -1, "Octaves");
			ASSERT(lua_isinteger(l, -1), "Invalid lua script(Noise)");
			Octaves = (int)lua_tointeger(l, -1);
			lua_pop(l, 1);

			lua_getfield(l, -1, "YLevelStretch");
			ASSERT(lua_isinteger(l, -1), "Invalid lua script(Noise)");
			YLevelStretch = (int)lua_tointeger(l, -1);
			lua_pop(l, 1);

			lua_getfield(l, -1, "Frequency");
			ASSERT(lua_isnumber(l, -1), "Invalid lua script(Noise)");
			Frequency = lua_tonumber(l, -1);
			lua_pop(l, 1);

			lua_getfield(l, -1, "BiomeStretch");
			ASSERT(lua_isinteger(l, -1), "Invalid lua script(Noise)");
			BiomeStretch = (int)lua_tointeger(l, -1);
			lua_pop(l, 1);
			Noise::SetNoiseSettings(Octaves, Frequency, YLevelStretch, BiomeStretch);
		}
	}
	//GetWorldGenerationProperties
	{
		lua_getglobal(l, "WorldGeneration");
		if (lua_istable(l, -1))
		{
			lua_getfield(l, -1, "Filler");
			ASSERT(lua_isstring(l, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::FillerBlock = Block::GetBlockType(lua_tostring(l,-1));
			lua_pop(l, 1);

			lua_getfield(l, -1, "Underground");
			ASSERT(lua_isstring(l, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::UndergroundBlock = Block::GetBlockType(lua_tostring(l, -1));
			lua_pop(l, 1);

			lua_getfield(l, -1, "Dirt");
			ASSERT(lua_isstring(l, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::DirtBlock = Block::GetBlockType(lua_tostring(l, -1));
			lua_pop(l, 1);

			lua_getfield(l, -1, "DryTop");
			ASSERT(lua_isstring(l, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::DryTopBlock = Block::GetBlockType(lua_tostring(l, -1));
			lua_pop(l, 1);

			lua_getfield(l, -1, "WetTop");
			ASSERT(lua_isstring(l, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::WetTopBlock = Block::GetBlockType(lua_tostring(l, -1));
			lua_pop(l, 1);

			lua_getfield(l, -1, "DeadTop");
			ASSERT(lua_isstring(l, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::DeadTopBlock = Block::GetBlockType(lua_tostring(l, -1));
			lua_pop(l, 1);

			lua_getfield(l, -1, "StoneTop");
			ASSERT(lua_isstring(l, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::StoneTopBlock = Block::GetBlockType(lua_tostring(l, -1));
			lua_pop(l, 1);

			lua_getfield(l, -1, "Ore");
			ASSERT(lua_isstring(l, -1), "Invalid lua script(WorldGenerationProperties)");
			Block::OreBlock = Block::GetBlockType(lua_tostring(l, -1));
			lua_pop(l, 1);
		}
	}
	//GetItemProperties
	{
		lua_getglobal(l, "Items");
		if (lua_istable(l, -1))
		{
			lua_pushnil(l);
			for (; lua_next(l, -2) != 0; lua_pop(l, 1)) {
				ItemProperties bp{};

				ASSERT(lua_isstring(l, -2), "Invalid lua script(items)");
				strcpy_s(bp.name.data(), bp.name.size() - 1, lua_tostring(l, -2));

				lua_getfield(l, -1, "Texture");
				ASSERT(lua_isinteger(l, -1), "Invalid lua script(items)");
				bp.texture = (unsigned char)lua_tointeger(l, -1);
				lua_pop(l, 1);



				Item::itemProperties.push_back(bp);
			}
		}
	}
	//lua_close(L);
}

void LuaManager::MouseEvent(uint64_t UUID, MouseState LeftMouse, MouseState RightMouse, MouseState MiddleMouse)
{
	if (LeftMouse == MouseState::Click)
	{
		lua_getglobal(l, "OnLeftMouseClickEvent");
		if (lua_isfunction(l, -1))
		{
			lua_pushinteger(l, UUID);
			lua_pcall(l, 1, 0, 0);
		}
	}
	if (RightMouse == MouseState::Click)
	{
		lua_getglobal(l, "OnRightMouseClickEvent");
		if (lua_isfunction(l, -1))
		{
			lua_pushinteger(l, UUID);
			lua_pcall(l, 1, 0, 0);
		}
	}
	if (MiddleMouse == MouseState::Click)
	{
		lua_getglobal(l, "OnMiddleMouseClickEvent");
		if (lua_isfunction(l, -1))
		{
			lua_pushinteger(l, UUID);
			lua_pcall(l, 1, 0, 0);
		}
	}
}
void LuaManager::UpdateEvent()
{
	lua_getglobal(l, "Update");
	if (lua_isfunction(l, -1))
	{
		lua_pcall(l, 0, 0, 0);
	}
}
