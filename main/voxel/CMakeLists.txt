
file(GLOB_RECURSE src_files include/*.h src/*.cpp)

add_library(voxel_lib STATIC ${src_files})
target_link_libraries(voxel_lib PUBLIC Neko_Core sdl_engine gles3_wrapper)
target_include_directories(voxel_lib PUBLIC include/)
file(GLOB_RECURSE main_files main/*.cpp)

data_generate(voxel_lib)

foreach(main_project_path ${main_files})
    get_filename_component(main_project_name ${main_project_path} NAME_WE)
    add_executable(${main_project_name} ${main_project_path})
    target_include_directories(${main_project_name} PUBLIC ${EXTERNAL_DIR}/utilities/)
    set_target_properties (${main_project_name} PROPERTIES FOLDER Neko/Main/Voxel)
    target_link_libraries(${main_project_name} PUBLIC voxel_lib)
endforeach()
