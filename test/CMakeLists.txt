

#Google Test
enable_testing()
set(GOOGLE_TEST_DIR ${EXTERNAL_DIR}/googletest-1.8.1)
set(BUILD_GMOCK OFF CACHE INTERNAL "")
set(INSTALL_GTEST OFF CACHE INTERNAL "")
add_subdirectory(${GOOGLE_TEST_DIR})
include_directories(${GOOGLE_TEST_DIR}/include)

set_target_properties (gtest gtest_main PROPERTIES
        FOLDER Externals\\GTest)

SET(Neko_TEST_DIR ${CMAKE_SOURCE_DIR}/test)
file(GLOB TEST_FILES ${Neko_TEST_DIR}/*.cpp)
add_executable(Neko_TEST ${TEST_FILES})

target_link_libraries(Neko_TEST gtest gtest_main Neko_COMMON ${EXTENSIONS_LIBS})
if (APPLE)
    target_link_libraries(Neko_TEST ${Boost_LIBRARIES})
else()
endif(APPLE)
add_dependencies(Neko_TEST DataTarget)

set_target_properties (Neko_TEST PROPERTIES FOLDER Neko)