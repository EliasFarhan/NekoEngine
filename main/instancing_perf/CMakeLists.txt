
file(GLOB_RECURSE src_files include/*.h src/*.cpp)
file(GLOB_RECURSE shader_files data/shaders/*.vert data/shaders/*.frag)
add_executable(instancing_perf ${src_files} ${shader_files})
target_link_libraries(instancing_perf PUBLIC Neko_Core sdl_engine gles3_wrapper sqlite3)
target_include_directories(instancing_perf PUBLIC include/)

set_target_properties (instancing_perf PROPERTIES FOLDER Neko/Main/)
data_generate(instancing_perf)
set_target_properties (instancing_perf_Generate_Data PROPERTIES FOLDER Neko/Main/Voxel)