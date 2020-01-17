
#sdl2
set(SDL_VERSION 2.0.9)
set(SDL_DIR ${EXTERNAL_DIR}/SDL2-${SDL_VERSION}/)
set(SDL_STATIC ON)
set(SDL_SHARED OFF)
add_subdirectory(${SDL_DIR})
include_directories(${SDL_DIR}/include ${SDL_DIR}/src)
LIST(APPEND engine3d_lib
        SDL2-static)
set_target_properties (SDL2-static PROPERTIES FOLDER Externals)
if(MSVC)
    LIST(APPEND engine3d_lib
            SDL2main)
    set_target_properties (SDL2main PROPERTIES FOLDER Externals)
endif(MSVC)

#imgui sdl
add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD=1)
set(IMGUI_ROOT ${EXTERNAL_DIR}/imgui-1.70/)
add_subdirectory(${SDL2_IMGUI})
include_directories(${IMGUI_ROOT})
LIST(APPEND engine3d_lib
        imgui-sdl)
set_target_properties (imgui-sdl PROPERTIES FOLDER Externals)