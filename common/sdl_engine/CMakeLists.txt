
if(NOT Emscripten)
if(Neko_Android)

    LIST(APPEND sdl_engine_libs SDL2 Neko_Core SDL2main)
else()
    LIST(APPEND sdl_engine_libs SDL2-static Neko_Core)
    set_target_properties (SDL2-static PROPERTIES FOLDER Externals)
    endif()
    if(MSVC)
        LIST(APPEND sdl_engine_libs
                SDL2main)
        set_target_properties (SDL2main PROPERTIES FOLDER Externals)
    endif(MSVC)
endif()

if(Neko_GLES3)
    LIST(APPEND sdl_engine_libs gles3_wrapper)
endif()

#imgui sdl
file(GLOB_RECURSE sdl_engine_src include/*.h src/*.cpp)

add_library(sdl_engine STATIC ${sdl_engine_src})
target_link_libraries(sdl_engine PUBLIC ${sdl_engine_libs} imgui)

target_include_directories(sdl_engine PUBLIC include/)
neko_lib_config(sdl_engine)
if(Emscripten)
    set_property(TARGET sdl_engine APPEND_STRING PROPERTY LINK_FLAGS " -s USE_SDL=2 -s USE_WEBGL2=1 ")
endif()
set_target_properties (sdl_engine PROPERTIES FOLDER Neko)