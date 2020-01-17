
LIST(APPEND sdl_engine_libs SDL2 Neko_Core)
set_target_properties (SDL2 PROPERTIES FOLDER Externals)
if(MSVC)
    LIST(APPEND sdl_engine_libs
            SDL2main)
    set_target_properties (SDL2main PROPERTIES FOLDER Externals)
endif(MSVC)

#imgui sdl
add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD=1)
include_directories(${IMGUI_ROOT})
include_directories("include/")
include_directories("${CMAKE_SOURCE_DIR}/core/include")
file(GLOB_RECURSE sdl_engine_src include/*.h src/*.cpp)

add_library(sdl_engine ${sdl_engine_src})
target_link_libraries(sdl_engine PUBLIC ${sdl_engine_libs} imgui)