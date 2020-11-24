
file(GLOB_RECURSE PHYSFS_WRAPPER_SRC src/*.cpp include/*h)
add_library(physfs_wrapper ${PHYSFS_WRAPPER_SRC})

target_include_directories(physfs_wrapper PUBLIC include/ ${PHYSFS_DIR}/src/)
target_link_libraries(physfs_wrapper PUBLIC physfs-static Neko_Core)

if(Neko_Test)
    add_neko_test(physfs_wrapper)
endif()
add_data_folder(physfs_wrapper)
set_target_properties (physfs_wrapper PROPERTIES FOLDER Neko/Common)
set_target_properties (physfs_wrapper_Copy_Data PROPERTIES FOLDER Neko/Common)
set_target_properties (physfs PROPERTIES FOLDER Externals)
set_target_properties (physfs-static PROPERTIES FOLDER Externals)
