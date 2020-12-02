

file(GLOB_RECURSE main_files main/*.cpp)

foreach(main_project_path ${main_files})
    get_filename_component(main_project_name ${main_project_path} NAME_WE)
    add_executable(${main_project_name} ${main_project_path})
    target_include_directories(${main_project_name} PUBLIC ${EXTERNAL_DIR}/utilities/)
    set_target_properties (${main_project_name} PROPERTIES FOLDER Neko/Main/Voxel)
endforeach()
