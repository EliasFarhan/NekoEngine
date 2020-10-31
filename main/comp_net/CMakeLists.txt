if (NOT Neko_GLES3 OR NOT Neko_SDL2 OR NOT Neko_SFML_NET)
    return()
endif()

include_directories("include/")

file(GLOB_RECURSE comp_net_src include/*.h src/*.cpp)

message("SFML LIBRARIES: ${SFML_LIBRARIES}")
add_library(comp_net_lib ${comp_net_src})
target_link_libraries(comp_net_lib PUBLIC Neko_Core sdl_engine gles3_wrapper assimp_wrapper sfml-network)
set_target_properties (comp_net_lib PROPERTIES FOLDER Neko/Main/CompNet)
set_target_properties(comp_net_lib PROPERTIES UNITY_BUILD ON)
set(net_main_project_dir "main/")
file(GLOB net_main_projects "${net_main_project_dir}/*")
neko_lib_config(comp_net_lib)

foreach(net_main_project_path ${net_main_projects} )
    # I used a simple string replace, to cut off .cpp.
    get_filename_component(net_main_project_name ${net_main_project_path} NAME_WE )

    message("Main project path: ${net_main_project_path} and name: ${net_main_project_name}")
    add_executable(${net_main_project_name} ${net_main_project_path})
    target_link_libraries(${net_main_project_name} PUBLIC  comp_net_lib )
    neko_bin_config(${net_main_project_name})


    if(EMSCRIPTEN)
        get_target_property(bin_link_flags ${net_main_project_name} LINK_FLAGS)
        set_target_properties(${net_main_project_name} PROPERTIES LINK_FLAGS
                "${bin_link_flags} -s USE_SDL=2 -s USE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2
                -lwebsocket.js -s PROXY_POSIX_SOCKETS=1 -s USE_PTHREADS=1 -s PROXY_TO_PTHREAD=1 --preload-file ${CMAKE_BINARY_DIR}/data/@ ")
        set_target_properties(${net_main_project_name} PROPERTIES SUFFIX ".html")
    endif()
    set_target_properties(${net_main_project_name} PROPERTIES UNITY_BUILD ON)
    if(NOT Emscripten)
        target_precompile_headers(${net_main_project_name} PRIVATE "include/comp_net/comp_net_pch.h")
    endif()
    set_target_properties (${net_main_project_name} PROPERTIES FOLDER Neko/Main/CompNet)
endforeach()