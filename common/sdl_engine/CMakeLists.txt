
LIST(APPEND sdl_engine_libs SDL2 Neko_Core)
set_target_properties (SDL2 PROPERTIES FOLDER Externals)
if(MSVC)
    LIST(APPEND sdl_engine_libs
            SDL2main)
    set_target_properties (SDL2main PROPERTIES FOLDER Externals)
endif(MSVC)

if(Neko_GLES3)
    LIST(APPEND sdl_engine_libs gles3_wrapper)
endif()

#imgui sdl
file(GLOB_RECURSE sdl_engine_src include/*.h src/*.cpp)

add_library(sdl_engine STATIC ${sdl_engine_src})
target_link_libraries(sdl_engine PUBLIC ${sdl_engine_libs} imgui)

target_include_directories(sdl_engine PUBLIC include/)