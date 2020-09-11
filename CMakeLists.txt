cmake_minimum_required(VERSION 3.16)
project(Neko)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS On)

include(cmake/neko_executable.cmake)


set(Neko_Profile OFF CACHE BOOL "Activate Profiling with Easy Profile")
set(Neko_GLES3 ON CACHE BOOL "Activate OpenGL ES 3.0")
set(Neko_SDL2 ON CACHE BOOL "Activate SDL2")
set(Neko_Box2D OFF CACHE BOOL "Activate Box2D")
set(Neko_Test ON CACHE BOOL "Activate Test")
set(Neko_Freetype ON CACHE BOOL "Activate Freetype")
set(Neko_Assert ON CACHE BOOL "Activate Neko Assertion")
set(Neko_Benchmark ON CACHE BOOL "Activate Benchmark")
set(Neko_Assimp ON CACHE BOOL "Activate Assimp Wrapper")
set(Neko_SFML_NET ON CACHE BOOL "Activate SFML Net Wrapper")
set(Neko_SameThread OFF CACHE BOOL "Activate Same Thread Rendering and Resource Loading")

MESSAGE("CMAKE SYSTEM NAME: ${CMAKE_SYSTEM_NAME}")

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Emscripten")
    include(cmake/platforms/emscripten.cmake)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
    set(Neko_SameThread ON CACHE BOOL "")
    mark_as_advanced(Neko_SameThread)
endif()

if(UNIX)
    EXECUTE_PROCESS(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
    MESSAGE("UNIX Architecture: ${ARCHITECTURE}")
    if("${ARCHITECTURE}" MATCHES "arm")
        include(cmake/platforms/arm.cmake)
    endif()
endif()

if(MSVC)
    include(cmake/platforms/msvc.cmake)
endif()

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/externals CACHE INTERNAL "Externals folder")
set(UTILITIES_DIR "${EXTERNAL_DIR}/utilities" CACHE INTERNAL "")

if(Neko_Assert)
    add_compile_definitions("NEKO_ASSERT=1")
endif()

if(Neko_SameThread)
    add_compile_definitions("NEKO_SAMETHREAD=1")
endif()


if(Neko_Profile)
    MESSAGE("Enable profiling")
    set(EASY_PROFILE_DIR "${EXTERNAL_DIR}/easy_profiler-2.1.0")
    add_compile_definitions("EASY_PROFILE_USE=1")
    add_subdirectory(${EASY_PROFILE_DIR})
    
    set_target_properties (easy_profiler PROPERTIES FOLDER Externals)
    set_target_properties (profiler_converter PROPERTIES FOLDER Externals)
    set_target_properties (profiler_sample PROPERTIES FOLDER Externals)
    set_target_properties (profiler_reader PROPERTIES FOLDER Externals)
    set_target_properties (profiler_sample_disabled_profiler PROPERTIES FOLDER Externals)
    

endif()

include("cmake/data.cmake")
set(NEKO_CORE_DIR "${CMAKE_SOURCE_DIR}/core/" CACHE INTERNAL "")
add_subdirectory(${NEKO_CORE_DIR})
if(NOT Emscripten)
target_precompile_headers(Neko_Core PRIVATE "${NEKO_CORE_DIR}/include/core_pch.h")
endif()
#imgui
set(IMGUI_VERSION "1.74" CACHE INTERNAL "")
set(IMGUI_ROOT "${EXTERNAL_DIR}/imgui-${IMGUI_VERSION}" CACHE STRING "")
include_directories("${IMGUI_ROOT}")
add_subdirectory("${IMGUI_ROOT}")
set_target_properties(imgui PROPERTIES UNITY_BUILD ON)


if(Neko_GLES3)
    add_compile_definitions("NEKO_GLES3=1")
    if(NOT Emscripten)
    add_subdirectory("${EXTERNAL_DIR}/glad")
    endif()
    set(GLES3_WRAPPER_DIR "common/gles3_wrapper" CACHE INTERNAL "")
    add_subdirectory(${GLES3_WRAPPER_DIR})
endif()

if(Neko_SDL2)
    #sdl2
    set(SDL_VERSION 2.0.9)
    set(SDL_DIR ${EXTERNAL_DIR}/SDL2-${SDL_VERSION}/)
    set(SDL_STATIC ON CACHE INTERNAL "")
    set(SDL_SHARED OFF CACHE INTERNAL "")
    set(SDL_ENGINE_DIR ON CACHE INTERNAL "")
    if(NOT Emscripten)
        add_subdirectory(${SDL_DIR})
    ENDIF()
    include_directories(${SDL_DIR}/include ${SDL_DIR}/src)

    add_subdirectory("common/sdl_engine")
    if(NOT Emscripten)
        target_precompile_headers(sdl_engine PRIVATE "common/sdl_engine/include/sdl_wrapper_pch.h")
    endif()
endif()

if(Neko_Assimp)
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
    add_subdirectory("common/assimp_wrapper")
    if(ZLIB_BUILD_ASSIMP)
        set_target_properties (zlib PROPERTIES FOLDER Externals)
        set_target_properties (zlibstatic PROPERTIES FOLDER Externals)
    endif()
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
    add_subdirectory("common/ft_wrapper")
endif()

if(Neko_Test)
    enable_testing()
    set(GOOGLE_TEST_DIR ${EXTERNAL_DIR}/googletest-1.8.1)
    set(BUILD_GMOCK OFF CACHE INTERNAL "")
    set(INSTALL_GTEST OFF CACHE INTERNAL "")
    add_subdirectory(${GOOGLE_TEST_DIR})
    include_directories(${GOOGLE_TEST_DIR}/include)
    add_subdirectory("test/")
    set_target_properties (gtest PROPERTIES FOLDER Externals/GTest)
    set_target_properties(gtest_main PROPERTIES FOLDER Externals/GTest)
endif()

if(Neko_Benchmark)
    #Google benchmark
    set(GOOGLE_BENCH_DIR "${EXTERNAL_DIR}/benchmark-1.5.0" CACHE STRING "")

    add_subdirectory(${GOOGLE_BENCH_DIR})
    add_subdirectory("benchmark/")
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
