cmake_minimum_required(VERSION 3.16)
project(Neko)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(Neko_Profile OFF CACHE BOOL "Activate Profiling with Easy Profile")
set(Neko_GLES3 ON CACHE BOOL "Activate OpenGL ES 3.0")
set(Neko_SDL2 ON CACHE BOOL "Activate SDL2")
set(Neko_Box2D OFF CACHE BOOL "Activate Box2D")
set(Neko_Test ON CACHE BOOL "Activate Test")
set(Neko_Assert ON CACHE BOOL "Activate Neko Assertion")
set(Neko_Benchmark ON CACHE BOOL "Activate Benchmark")
set(Neko_SFML_NET ON CACHE BOOL "Activate SFML Net Wrapper")
set(Neko_PhysFS ON CACHE BOOL "Activate PhysFS Wrapper")
set(Neko_sqlite3 ON CACHE BOOL "Activate sqlite3 dependency")

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
include(cmake/test.cmake)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/externals CACHE INTERNAL "Externals folder")
set(UTILITIES_DIR "${EXTERNAL_DIR}/utilities" CACHE INTERNAL "")


if(Neko_PhysFS)

    add_subdirectory(common/physfs_wrapper)
endif()

if(Neko_Profile)
    MESSAGE("Enable profiling")
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



if(Neko_GLES3)
    set(GLES3_WRAPPER_DIR "common/gles3_wrapper" CACHE INTERNAL "")
    add_subdirectory(${GLES3_WRAPPER_DIR})
endif()

if(Neko_SFML_NET)
	set(SFML_DIR "${EXTERNAL_DIR}/SFML-2.5.1" CACHE STRING "")
	include_directories("${SFML_DIR}/include")
	add_subdirectory("${SFML_DIR}")
endif()

if(Neko_Benchmark)
    add_subdirectory("benchmark/")
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
