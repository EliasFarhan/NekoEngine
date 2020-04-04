include_directories("include/")

file(GLOB_RECURSE triangle_src include/*.h src/*.cpp)

add_executable(triangle ${triangle_src})
target_link_libraries(triangle PUBLIC Neko_Core sdl_engine vulkan_wrapper)
neko_bin_config(triangle)

