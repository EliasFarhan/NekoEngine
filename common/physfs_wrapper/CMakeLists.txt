
file(GLOB_RECURSE PHYSFS_WRAPPER_SRC src/*.cpp include/*h)
add_library(physfs_wrapper ${PHYSFS_WRAPPER_SRC})

target_include_directories(physfs_wrapper PUBLIC include/ ${PHYSFS_DIR}/src/)
target_link_libraries(physfs_wrapper PUBLIC physfs-static Neko_Core)
add_test(physfs_wrapper)
add_data_folder(physfs_wrapper)