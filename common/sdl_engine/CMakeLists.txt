

if(Neko_Android)
    LIST(APPEND sdl_engine_libs SDL2-static Neko_Core SDL2main)
else()
    find_package(SDL2 CONFIG REQUIRED)

    LIST(APPEND sdl_engine_libs Neko_Core SDL2::SDL2 SDL2::SDL2main )
endif()


#imgui sdl
file(GLOB_RECURSE sdl_engine_src include/*.h src/*.cpp)

add_library(sdl_engine STATIC ${sdl_engine_src})
target_link_libraries(sdl_engine PUBLIC ${sdl_engine_libs})

target_include_directories(sdl_engine PUBLIC "include/")
target_precompile_headers(sdl_engine PUBLIC "include/sdl_wrapper_pch.h")
neko_lib_config(sdl_engine)
set_target_properties (sdl_engine PROPERTIES FOLDER Neko/Common)

