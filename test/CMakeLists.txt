

#Google Test


SET(Neko_TEST_DIR ${CMAKE_SOURCE_DIR}/test)
file(GLOB TEST_FILES ${Neko_TEST_DIR}/*.cpp)
if(Neko_PhysFS)
    file(GLOB PHYSFS_TEST_FILES ${Neko_TEST_DIR}/physfs/*.cpp)
endif()
add_executable(Neko_TEST ${TEST_FILES} ${PHYSFS_TEST_FILES})

include_directories(${NEKO_CORE_DIR}/include)

target_link_libraries(Neko_TEST gtest gtest_main Neko_Core sdl_engine gles3_wrapper)

if(Neko_PhysFS)
    target_link_libraries(Neko_TEST physfs_wrapper)
endif()

neko_bin_config(Neko_TEST)
if(UNIX)
#target_compile_options(Neko_TEST PUBLIC "--coverage")
#target_link_options(Neko_TEST PUBLIC "--coverage")
endif()
set_target_properties (Neko_TEST PROPERTIES FOLDER Neko)