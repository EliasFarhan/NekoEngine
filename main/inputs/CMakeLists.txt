include_directories("include/")

file(GLOB_RECURSE input_engine_src include/*.h src/*.cpp)

add_executable(input_engine ${input_engine_src})
target_link_libraries(input_engine PUBLIC Neko_Core sdl_engine gles3_wrapper)
neko_bin_config(input_engine)

if(EMSCRIPTEN)
    get_target_property(bin_link_flags input_engine LINK_FLAGS)
    set_target_properties(input_engine PROPERTIES LINK_FLAGS
            "${bin_link_flags} -s USE_SDL=2 -s USE_WEBGL2=1 ")
endif()

set_target_properties (input_engine PROPERTIES FOLDER Neko)