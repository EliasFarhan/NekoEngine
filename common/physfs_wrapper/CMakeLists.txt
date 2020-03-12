
if(NOT Emscripten)
    LIST(APPEND physfs Neko_Core)
    set_target_properties (physfs PROPERTIES FOLDER Externals)
endif()

LIST(APPEND physfs_wrapper_libs physfs)

#imgui sdl
file(GLOB_RECURSE physfs_wrapper_src include/*.h src/*.cpp)

add_library(physfs_wrapper STATIC ${physfs_wrapper_src})
target_link_libraries(physfs_wrapper PUBLIC ${physfs_wrapper_libs} Neko_Core)

target_include_directories(physfs_wrapper PUBLIC include/)
neko_lib_config(physfs_wrapper)
set_target_properties (physfs_wrapper PROPERTIES FOLDER Neko)