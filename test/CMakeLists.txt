

#Google Test


SET(Neko_TEST_DIR ${CMAKE_SOURCE_DIR}/test)
file(GLOB TEST_FILES ${Neko_TEST_DIR}/*.cpp)
add_executable(Neko_TEST ${TEST_FILES})

include_directories(${NEKO_CORE_DIR}/include)

target_link_libraries(Neko_TEST gtest gtest_main Neko_Core)


set_target_properties (Neko_TEST PROPERTIES FOLDER Neko)