include_directories("include/")

file(GLOB_RECURSE comp_net_src include/*.h src/*.cpp)

add_executable(comp_net ${comp_net_src} )
target_link_libraries(comp_net PUBLIC Neko_Core sdl_engine gles3_wrapper assimp_wrapper ${SFML_LIBRARIES})
neko_bin_config(comp_net)

if(EMSCRIPTEN)
    get_target_property(bin_link_flags comp_net LINK_FLAGS)
    set_target_properties(comp_net PROPERTIES LINK_FLAGS
            "${bin_link_flags} -s USE_SDL=2 -s USE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2")
endif()
set_target_properties(comp_net PROPERTIES UNITY_BUILD ON)
if(NOT Emscripten)
    target_precompile_headers(comp_net PRIVATE "include/comp_net/comp_net_pch.h")
endif()
set_target_properties (comp_net PROPERTIES FOLDER Neko/Main)