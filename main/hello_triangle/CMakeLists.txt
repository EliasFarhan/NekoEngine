include_directories("include/")

file(GLOB_RECURSE hello_triangle_src include/*.h src/*.cpp)

add_executable(hello_triangle ${hello_triangle_src})
target_link_libraries(hello_triangle PUBLIC Neko_Core sdl_engine gles3_wrapper)