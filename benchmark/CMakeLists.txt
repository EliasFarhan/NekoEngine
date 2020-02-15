include_directories(${GOOGLE_BENCH_DIR}/include)
set_target_properties (benchmark benchmark_main PROPERTIES
        FOLDER Externals\\GTest)

SET(BENCH_DIR ${CMAKE_SOURCE_DIR}/benchmark)
file(GLOB BENCH_FILES ${BENCH_DIR}/*.cpp)
neko_lib_config(benchmark)
neko_lib_config(benchmark_main)
include_directories("include/")
foreach(bench_file ${BENCH_FILES} )
    # I used a simple string replace, to cut off .cpp.
    file(RELATIVE_PATH bench_relative_path ${BENCH_DIR} ${bench_file})
    string( REPLACE ".cpp" "" bench_name ${bench_relative_path} )
    add_executable(${bench_name} ${bench_file})
    # Make sure YourLib is linked to each app
    target_link_libraries(${bench_name} PUBLIC benchmark benchmark_main Neko_Core )

    IF(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Oi /Ox /fp:fast /FAs /arch:AVX2")
    elseif(UNIX)
        set_target_properties(${bench_name} PROPERTIES COMPILE_FLAGS
                "-save-temps -march=haswell -ffast-math -fno-omit-frame-pointer -fno-exceptions -fno-rtti")
    ENDIF(MSVC)
    neko_bin_config(${bench_name})
    set_target_properties (${bench_name} PROPERTIES
            FOLDER Benchmark)
    IF(APPLE)
        set_target_properties(${bench_name} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
                RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
    ENDIF()
endforeach()