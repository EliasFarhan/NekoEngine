include_directories("include/")

file(GLOB_RECURSE comp_graph_src include/*.h src/*.cpp)

add_executable(comp_graph ${comp_graph_src})
target_link_libraries(comp_graph PUBLIC Neko_Core sdl_engine gles3_wrapper)