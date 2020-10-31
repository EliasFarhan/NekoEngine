if (NOT Neko_GLES3 OR NOT Neko_SDL2 OR NOT Neko_Assimp OR NOT Neko_KTX OR NOT Neko_Freetype)
    return()
endif()
include_directories("include/")

file(GLOB_RECURSE comp_graph_src include/*.h src/*.cpp)

file(GLOB_RECURSE shaders_src ${CMAKE_SOURCE_DIR}/data/shaders/*.vert ${CMAKE_SOURCE_DIR}/data/shaders/*.frag)

source_group("Shaders"	FILES ${shaders_src})

add_executable(comp_graph ${comp_graph_src} ${shaders_src})
target_link_libraries(comp_graph PUBLIC Neko_Core sdl_engine gles3_wrapper assimp_wrapper ktx)
neko_bin_config(comp_graph)

if(EMSCRIPTEN)
    get_target_property(bin_link_flags comp_graph LINK_FLAGS)
    set_target_properties(comp_graph PROPERTIES LINK_FLAGS
            "${bin_link_flags} -s USE_SDL=2 -s USE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 --preload-file ${CMAKE_BINARY_DIR}/data/@ ")
    set_target_properties(comp_graph PROPERTIES SUFFIX ".html")
endif()
set_target_properties(comp_graph PROPERTIES UNITY_BUILD ON)

if(NOT Emscripten)
target_precompile_headers(comp_graph PRIVATE "include/comp_graph/comp_graph_pch.h")
endif()
set_target_properties (comp_graph PROPERTIES FOLDER Neko/Main)