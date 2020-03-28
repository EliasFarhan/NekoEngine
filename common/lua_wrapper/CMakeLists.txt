
add_compile_definitions("LUAPP_HEADER_ONLY=1")

include_directories(${UTILITIES_DIR})

message("LUA WRAPPER DIR: ${LUA_WRAPPER_DIR}")
file(GLOB_RECURSE lua_wrapper_src ${LUA_WRAPPER_DIR}/include/*.h ${LUA_WRAPPER_DIR}/src/*.cpp)
add_library(lua_wrapper STATIC ${lua_wrapper_src})
target_include_directories(lua_wrapper PUBLIC ${LUA_WRAPPER_DIR}/include)
target_link_libraries(lua_wrapper lua)
