#include "lua_engine.h"
#include <gtest/gtest.h>

TEST(Engine, TestLua)
{
	neko::LuaEngine luaEngine;
	luaEngine.InterpretLine("print(\"Hello From Lua\")");
}