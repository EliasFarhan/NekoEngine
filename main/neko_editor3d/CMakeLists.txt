file(GLOB_RECURSE neko_editor3d_src include/*.h src/*.cpp)

add_executable(neko_editor3d ${neko_editor3d_src})
target_include_directories(neko_editor3d PUBLIC ${Python3_INCLUDE_DIRS})
target_include_directories(neko_editor3d PUBLIC "include/")
target_link_directories(neko_editor3d PUBLIC ${Python3_LIBRARY_DIRS})
target_link_libraries(neko_editor3d PUBLIC Neko_Core sdl_engine gles3_wrapper pybind11::embed ${Python3_LIBRARIES})
target_precompile_headers(neko_editor3d PRIVATE "include/editor_pch.h")

set_target_properties (neko_editor3d PROPERTIES FOLDER Neko/Main)
set_target_properties(neko_editor3d PROPERTIES UNITY_BUILD ON)