find_package(PhysFS REQUIRED)

file(GLOB_RECURSE PHYSFS_WRAPPER_SRC src/*.cpp include/*h)
add_library(physfs_wrapper STATIC ${PHYSFS_WRAPPER_SRC})

target_include_directories(physfs_wrapper PUBLIC include/)
target_link_libraries(physfs_wrapper PUBLIC ${PHYSFS_LIBRARY} Neko_Core)
MESSAGE("PHYFS LIBRARY: ${PHYSFS_LIBRARY}")
if(Neko_Test)
    add_neko_test(physfs_wrapper)
endif()
add_data_folder(physfs_wrapper)
set_target_properties (physfs_wrapper PROPERTIES FOLDER Neko/Common)
set_target_properties (physfs_wrapper_Copy_Data PROPERTIES FOLDER Neko/Common)
