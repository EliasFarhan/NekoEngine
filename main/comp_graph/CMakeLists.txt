if (NOT Neko_GLES3 OR NOT Neko_SDL2 OR NOT Neko_KTX OR NOT Neko_Freetype)
    return()
endif()
include_directories("include/")

file(GLOB_RECURSE comp_graph_src include/*.h src/*.cpp)

file(GLOB_RECURSE shaders_src ${CMAKE_SOURCE_DIR}/data/shaders/*.vert ${CMAKE_SOURCE_DIR}/data/shaders/*.frag)

source_group("Shaders"	FILES ${shaders_src})
add_library(comp_graph_lib STATIC ${comp_graph_src} ${shaders_src})
# TODO list alll main files to create a new executable
add_executable(comp_graph main/main.cpp)
target_link_libraries(comp_graph PUBLIC comp_graph_lib)
target_link_libraries(comp_graph_lib PUBLIC Neko_Core sdl_engine gles3_wrapper ktx)
neko_lib_config(comp_graph_lib)
neko_bin_config(comp_graph)
data_generate(comp_graph_lib)
set_target_properties(comp_graph_lib PROPERTIES UNITY_BUILD ON)

target_precompile_headers(comp_graph_lib PRIVATE "include/comp_graph/comp_graph_pch.h")

set_target_properties (comp_graph_lib PROPERTIES FOLDER Neko/Main/CompGraph)
set_target_properties (comp_graph_lib_Generate_Data PROPERTIES FOLDER Neko/Main/CompGraph)
set_target_properties (comp_graph PROPERTIES FOLDER Neko/Main/CompGraph)

if(Neko_Test)
    add_neko_test(comp_graph_lib)
endif()