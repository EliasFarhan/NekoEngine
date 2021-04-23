if (NOT Neko_Vulkan OR NOT Neko_SDL2)
    return()
endif()


file(GLOB_RECURSE vk_comp_graph_src include/*.h src/*.cpp)

file(GLOB_RECURSE shaders_src "data/shaders/*.vert" "data/shaders/*.frag")

#source_group("Shaders" FILES "${shaders_src}")
add_library(vk_comp_graph_lib STATIC "${vk_comp_graph_src}" "${shaders_src}")
add_executable(vk_comp_graph main/main.cpp)
target_include_directories(vk_comp_graph_lib PUBLIC "include/")
target_link_libraries(vk_comp_graph PUBLIC vk_comp_graph_lib)
target_link_libraries(vk_comp_graph_lib PUBLIC Neko_Core sdl_engine vk_wrapper)
neko_lib_config(vk_comp_graph_lib)
neko_bin_config(vk_comp_graph)
data_generate(vk_comp_graph_lib)
set_target_properties(vk_comp_graph_lib PROPERTIES UNITY_BUILD ON)

set_target_properties (vk_comp_graph_lib PROPERTIES FOLDER Neko/Main/CompGraph)
set_target_properties (vk_comp_graph_lib_Generate_Data PROPERTIES FOLDER Neko/Main/CompGraph)
set_target_properties (vk_comp_graph PROPERTIES FOLDER Neko/Main/CompGraph)
