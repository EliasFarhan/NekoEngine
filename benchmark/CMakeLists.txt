include_directories(${GOOGLE_BENCH_DIR}/include)
set_target_properties (benchmark benchmark_main PROPERTIES
        FOLDER Externals\\GTest)

SET(BENCH_DIR ${CMAKE_SOURCE_DIR}/benchmark)
file(GLOB BENCH_FILES ${BENCH_DIR}/*.cpp)
include_directories("include/")
foreach(bench_file ${BENCH_FILES} )
    # I used a simple string replace, to cut off .cpp.
    file(RELATIVE_PATH bench_relative_path ${BENCH_DIR} ${bench_file})
    string( REPLACE ".cpp" "" bench_name ${bench_relative_path} )
    add_executable(${bench_name} ${bench_file})
    # Make sure YourLib is linked to each app
    target_link_libraries(${bench_name} PUBLIC benchmark benchmark_main Neko_Core )
    neko_bin_config(${bench_name})
    IF(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Oi /Ox /fp:fast /FAs /arch:AVX2")
    elseif(UNIX AND NOT EMSCRIPTEN)
        set_property(TARGET ${bench_name} APPEND_STRING PROPERTY COMPILE_FLAGS " -fno-omit-frame-pointer -save-temps ")
    ENDIF(MSVC)
    if(EMSCRIPTEN)
        set_target_properties(${bench_name} PROPERTIES SUFFIX ".js")
    endif()
    set_target_properties (${bench_name} PROPERTIES
            FOLDER Neko/Benchmark)
    IF(APPLE)
        set_target_properties(${bench_name} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
                RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
    ENDIF()
endforeach()
