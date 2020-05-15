include_directories("include/")

file(GLOB_RECURSE comp_graph_src include/*.h src/*.cpp)

add_executable(comp_graph ${comp_graph_src})
target_link_libraries(comp_graph PUBLIC Neko_Core sdl_engine gles3_wrapper assimp_wrapper)
neko_bin_config(comp_graph)

if(EMSCRIPTEN)
    get_target_property(bin_link_flags comp_graph LINK_FLAGS)
    set_target_properties(comp_graph PROPERTIES LINK_FLAGS
            "${bin_link_flags} -s USE_SDL=2 -s USE_WEBGL2=1 ")
endif()
set_target_properties(comp_graph PROPERTIES UNITY_BUILD ON)
if(NOT Emscripten)
target_precompile_headers(comp_graph PRIVATE "include/comp_graph/comp_graph_pch.h")
endif()
set_target_properties (comp_graph PROPERTIES FOLDER Neko\\Main)