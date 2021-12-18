file(GLOB_RECURSE comp_graph_src include/*.h src/*.cpp)

file(GLOB_RECURSE shaders_src data/shaders/*.vert data/shaders/*.frag data/shaders/*.comp)

source_group("Shaders"	FILES ${shaders_src})
add_library(comp_graph_lib STATIC ${comp_graph_src} ${shaders_src})
add_executable(comp_graph main/main.cpp)
target_include_directories(comp_graph_lib PUBLIC "include/")

target_link_libraries(comp_graph_lib PUBLIC Neko_Core sdl_engine gles3_wrapper)
neko_lib_config(comp_graph_lib)
data_generate(comp_graph_lib)
set_target_properties(comp_graph_lib PROPERTIES UNITY_BUILD ON)
set_target_properties (comp_graph_lib PROPERTIES FOLDER Neko/Main/CompGraph)
set_target_properties (comp_graph_lib_Generate_Data PROPERTIES FOLDER Neko/Main/CompGraph)


target_link_libraries(comp_graph PUBLIC comp_graph_lib)
target_precompile_headers(comp_graph_lib PRIVATE "include/comp_graph/comp_graph_pch.h")
neko_bin_config(comp_graph)

set_target_properties (comp_graph PROPERTIES FOLDER Neko/Main/CompGraph)

if(Neko_Test)
    add_neko_test(comp_graph_lib)
endif()