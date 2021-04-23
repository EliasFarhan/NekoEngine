cmake_minimum_required(VERSION 3.16)
project(Neko)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


add_compile_definitions("FMT_HEADER_ONLY=1")

set(Neko_Profile OFF CACHE BOOL "Activate Profiling with Easy Profile")
set(Neko_GLES3 ON CACHE BOOL "Activate OpenGL ES 3.0")
set(Neko_SDL2 ON CACHE BOOL "Activate SDL2")
set(Neko_Box2D OFF CACHE BOOL "Activate Box2D")
set(Neko_Test ON CACHE BOOL "Activate Test")
set(Neko_Freetype ON CACHE BOOL "Activate Freetype")
set(Neko_Assert ON CACHE BOOL "Activate Neko Assertion")
set(Neko_Benchmark ON CACHE BOOL "Activate Benchmark")
set(Neko_SFML_NET ON CACHE BOOL "Activate SFML Net Wrapper")
set(Neko_PhysFS ON CACHE BOOL "Activate PhysFS Wrapper")
set(Neko_sqlite3 ON CACHE BOOL "Activate sqlite3 dependency")
set(Neko_Vulkan ON CACHE BOOL "Activate Vulkan Wrapper")
set(Neko_Assimp ON CACHE BOOL "Activate Assimp Dependencies")

MESSAGE("CMAKE SYSTEM NAME: ${CMAKE_SYSTEM_NAME}")

if(UNIX)
    EXECUTE_PROCESS(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
    MESSAGE("UNIX Architecture: ${ARCHITECTURE}")
    if("${ARCHITECTURE}" MATCHES "arm")
        include(cmake/platforms/arm.cmake)
    endif()
    if("${ARCHITECTURE}" MATCHES "aarch")
        include(cmake/platforms/arm.cmake)
    endif()
endif()

if(MSVC)
    include(cmake/platforms/msvc.cmake)
endif()

include(cmake/neko_executable.cmake)
include(cmake/data.cmake)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/externals CACHE INTERNAL "Externals folder")
set(UTILITIES_DIR "${EXTERNAL_DIR}/utilities" CACHE INTERNAL "")

if(Neko_Assert)
    add_compile_definitions("NEKO_ASSERT=1")
endif()

if(Neko_sqlite3)
    set(SQLITE3_VERSION "3.34.0")
    set(SQLITE3_PATH "${EXTERNAL_DIR}/sqlite3-${SQLITE3_VERSION}")
    add_subdirectory("${SQLITE3_PATH}")
endif()

if(Neko_Test)
    enable_testing()
    set(GOOGLE_TEST_DIR "${EXTERNAL_DIR}/googletest-1.8.1")
    set(BUILD_GMOCK OFF CACHE INTERNAL "")
    set(INSTALL_GTEST OFF CACHE INTERNAL "")
    add_subdirectory("${GOOGLE_TEST_DIR}")
    include(cmake/test.cmake)
    set_target_properties (gtest PROPERTIES FOLDER Externals/GTest)
    set_target_properties(gtest_main PROPERTIES FOLDER Externals/GTest)
endif()

if(Neko_PhysFS)
    set(PHYSFS_DIR "${EXTERNAL_DIR}/physfs-3.0.2" CACHE STRING "")
    add_subdirectory("${PHYSFS_DIR}")

    add_subdirectory(common/physfs_wrapper)
endif()

if(Neko_Profile)
    MESSAGE("Enable profiling")
    set(EASY_PROFILE_DIR "${EXTERNAL_DIR}/easy_profiler-2.1.0")
    add_compile_definitions("EASY_PROFILE_USE=1")
    add_subdirectory("${EASY_PROFILE_DIR}")
    
    set_target_properties (easy_profiler PROPERTIES FOLDER Externals)
    set_target_properties (profiler_converter PROPERTIES FOLDER Externals)
    set_target_properties (profiler_sample PROPERTIES FOLDER Externals)
    set_target_properties (profiler_reader PROPERTIES FOLDER Externals)
    set_target_properties (profiler_sample_disabled_profiler PROPERTIES FOLDER Externals)
    

endif()

set(tools_project_dir "tools/")
file(GLOB tools_projects "${tools_project_dir}/*")

foreach(tools_project_path ${tools_projects} )
    # I used a simple string replace, to cut off .cpp.
    get_filename_component(tools_project_name "${tools_project_path}" NAME )

    message("Tools project path: ${tools_project_path} and name: ${tools_project_name}")
    set(${tools_project_name} ON CACHE BOOL "")

    if(${tools_project_name})
        add_subdirectory("${tools_project_path}")
    endif()
endforeach()


include("cmake/data_generation.cmake")
set(NEKO_CORE_DIR "${CMAKE_SOURCE_DIR}/core/" CACHE INTERNAL "")
add_subdirectory(${NEKO_CORE_DIR})

target_precompile_headers(Neko_Core PRIVATE "${NEKO_CORE_DIR}/include/core_pch.h")



#pybind11
set(PYBIND11_VERSION "2.6.1")
set(PYBIND11_DIR "${EXTERNAL_DIR}/pybind11-${PYBIND11_VERSION}")
add_subdirectory("${PYBIND11_DIR}")




if(Neko_SDL2)
    #sdl2
	
	add_compile_definitions("NEKO_SDL2=1")
    set(SDL_VERSION 2.0.12)
    set(SDL_DIR "${EXTERNAL_DIR}/SDL2-${SDL_VERSION}/")
    set(SDL_STATIC ON CACHE INTERNAL "")
    set(SDL_SHARED OFF CACHE INTERNAL "")
    set(SDL_ENGINE_DIR ON CACHE INTERNAL "")
    add_subdirectory(${SDL_DIR})

    add_subdirectory("common/sdl_engine")
    target_include_directories(sdl_engine PUBLIC ${SDL_DIR}/include ${SDL_DIR}/src)
    target_precompile_headers(sdl_engine PRIVATE "common/sdl_engine/include/sdl_wrapper_pch.h")

endif()

if(Neko_Assimp)

    add_compile_definitions("NEKO_ASSIMP=1")
	set(ASSIMP_VERSION 5.0.1)
    set(ASSIMP_DIR "${EXTERNAL_DIR}/assimp-${ASSIMP_VERSION}")
    set(ASSIMP_NO_EXPORT ON CACHE INTERNAL "")
    set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE INTERNAL "")
    set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE INTERNAL "")
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "")
    set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")
    set(ASSIMP_INSTALL OFF CACHE INTERNAL "")
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

    SET (ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
    SET (ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
    SET (ASSIMP_BUILD_BLEND_IMPORTER ON CACHE BOOL "" FORCE)
    SET (ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)

    add_subdirectory("${ASSIMP_DIR}")
    set_target_properties (assimp IrrXML PROPERTIES
            FOLDER Externals)
endif()

if(Neko_GLES3)
    
    add_compile_definitions("NEKO_GLES3=1")
    add_subdirectory("${EXTERNAL_DIR}/glad")
    set(GLES3_WRAPPER_DIR "common/gles3_wrapper" CACHE INTERNAL "")
    add_subdirectory(${GLES3_WRAPPER_DIR})
endif()

if(Neko_Vulkan)
	add_compile_definitions("NEKO_VULKAN=1")
	 set(VULKAN_WRAPPER_DIR "common/vulkan_wrapper" CACHE INTERNAL "")
    add_subdirectory(${VULKAN_WRAPPER_DIR})
endif()

if(Neko_SFML_NET)
	set(SFML_DIR "${EXTERNAL_DIR}/SFML-2.5.1" CACHE STRING "")
	include_directories("${SFML_DIR}/include")
	add_subdirectory("${SFML_DIR}")
endif()

if(Neko_Freetype)
    set(FREETYPE_DIR "${EXTERNAL_DIR}/freetype-2.10.0" CACHE STRING "")
    include_directories("${FREETYPE_DIR}")
    add_subdirectory("${FREETYPE_DIR}")
    set_target_properties (freetype PROPERTIES FOLDER Externals)
endif()

if(Neko_Benchmark)
    #Google benchmark
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "")
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "")
    set(GOOGLE_BENCH_DIR "${EXTERNAL_DIR}/benchmark-1.5.2" CACHE STRING "")
    set(HAVE_STD_REGEX ON CACHE BOOL "")
    add_subdirectory(${GOOGLE_BENCH_DIR})
    add_subdirectory("benchmark/")
    neko_lib_config(benchmark)
    set_target_properties (benchmark PROPERTIES FOLDER Externals/GTest)
    set_target_properties(benchmark_main PROPERTIES FOLDER Externals/GTest)
endif()

set(main_project_dir "main/")
file(GLOB main_projects "${main_project_dir}/*")

foreach(main_project_path ${main_projects} )
    # I used a simple string replace, to cut off .cpp.
    get_filename_component(main_project_name ${main_project_path} NAME )

    message("Main project path: ${main_project_path} and name: ${main_project_name}")
    set(${main_project_name} ON CACHE BOOL "")

    if(${main_project_name})
        add_subdirectory(${main_project_path})
    endif()
endforeach()
