if (NOT Neko_GLES3 OR NOT Neko_SDL2 OR NOT Neko_KTX OR NOT Neko_Freetype)
    return()
endif()
include_directories("include/")

file(GLOB_RECURSE comp_graph_src include/*.h src/*.cpp)

file(GLOB_RECURSE shaders_src ${CMAKE_SOURCE_DIR}/data/shaders/*.vert ${CMAKE_SOURCE_DIR}/data/shaders/*.frag)

source_group("Shaders"	FILES ${shaders_src})

add_executable(comp_graph ${comp_graph_src} ${shaders_src})
target_link_libraries(comp_graph PUBLIC Neko_Core sdl_engine gles3_wrapper ktx)
neko_bin_config(comp_graph)
data_generate(comp_graph)
set_target_properties(comp_graph PROPERTIES UNITY_BUILD ON)

if(NOT Emscripten)
target_precompile_headers(comp_graph PRIVATE "include/comp_graph/comp_graph_pch.h")
endif()
set_target_properties (comp_graph PROPERTIES FOLDER Neko/Main/CompGraph)
set_target_properties (comp_graph_Generate_Data PROPERTIES FOLDER Neko/Main/CompGraph)
