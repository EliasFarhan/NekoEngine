file(GLOB lua_src ${LUA_DIR}/src/*.c ${LUA_DIR}/src/*.h ${LUA_DIR}/src/*.hpp)
add_library(lua STATIC ${lua_src})
target_include_directories(lua PUBLIC src/)