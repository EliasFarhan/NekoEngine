include_directories("include/")

file(GLOB_RECURSE logger_src include/*.h src/*.cpp)

add_executable(logger ${logger_src})
target_link_libraries(logger PUBLIC Neko_Core sdl_engine gles3_wrapper)
neko_bin_config(logger)

if(EMSCRIPTEN)
    get_target_property(bin_link_flags logger LINK_FLAGS)
    set_target_properties(logger PROPERTIES LINK_FLAGS
            "${bin_link_flags} -s USE_SDL=2 -s USE_WEBGL2=1 ")
endif()

set_target_properties (logger PROPERTIES FOLDER Neko)