include_directories("include/")

file(GLOB_RECURSE neko_editor3d_src include/*.h src/*.cpp)

add_executable(neko_editor3d ${neko_editor3d_src})
target_link_libraries(neko_editor3d PUBLIC Neko_Core sdl_engine gles3_wrapper assimp_wrapper)
neko_bin_config(neko_editor3d)

if(EMSCRIPTEN)
    get_target_property(bin_link_flags neko_editor3d LINK_FLAGS)
    set_target_properties(neko_editor3d PROPERTIES LINK_FLAGS
            "${bin_link_flags} -s USE_SDL=2 -s USE_WEBGL2=1 ")
endif()
set_target_properties (neko_editor3d PROPERTIES FOLDER Neko/Main)
set_target_properties(neko_editor3d PROPERTIES UNITY_BUILD ON)