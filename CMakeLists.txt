cmake_minimum_required(VERSION 3.16)

option(Neko_Profile "Activate Profiling with Easy Profile" OFF )
option(Neko_GLES3 "Activate OpenGL ES 3.0" ON)
option(Neko_SDL2 "Activate SDL2" ON)
option(Neko_Box2D "Activate Box2D" ON)
option(Neko_Test "Activate Test" ON )
option(Neko_Assert "Activate Neko Assertion" ON)
option(Neko_Benchmark  "Activate Benchmark" ON)
option(Neko_SFML_NET  "Activate SFML Net Wrapper" ON)
option(Neko_PhysFS "Activate PhysFS Wrapper" ON)
option(Neko_Assimp "Activate Assimp Wrapper" ON)
option(Neko_sqlite3 "Activate sqlite3 dependency" ON)

if(Neko_sqlite3)
    list(APPEND VCPKG_MANIFEST_FEATURES "sqlite")
endif()
if(Neko_Test)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()
if(Neko_Benchmark)
    list(APPEND VCPKG_MANIFEST_FEATURES "benchmarks")
endif()
if(Neko_GLES3)
    list(APPEND VCPKG_MANIFEST_FEATURES "gl")
endif()
if(Neko_PhysFS)
    list(APPEND VCPKG_MANIFEST_FEATURES "physfs")
endif()
if(Neko_Box2D)
    list(APPEND VCPKG_MANIFEST_FEATURES "box2d")
endif()
if(Neko_Assimp)
    list(APPEND VCPKG_MANIFEST_FEATURES "assimp")
endif()


project(Neko)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

MESSAGE("CMAKE SYSTEM NAME: ${CMAKE_SYSTEM_NAME}")

if(UNIX)
    EXECUTE_PROCESS(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
    MESSAGE("UNIX Architecture: ${ARCHITECTURE}")
    MESSAGE("VCPKG_TARGET: ${VCPKG_TARGET_TRIPLET}")
    if("${ARCHITECTURE}" MATCHES "arm" OR VCPKG_TARGET_TRIPLET MATCHES "arm")
        include(cmake/platforms/arm.cmake)
    elseif("${ARCHITECTURE}" MATCHES "aarch")
        include(cmake/platforms/arm.cmake)
    else()
        include(cmake/platforms/linux.cmake)
    endif()
endif()

if(MSVC)
    include(cmake/platforms/msvc.cmake)
endif()

include(cmake/neko_executable.cmake)
include(cmake/data.cmake)
include(cmake/test.cmake)
include(cmake/proto.cmake)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/externals CACHE INTERNAL "Externals folder")
set(UTILITIES_DIR "${EXTERNAL_DIR}/utilities" CACHE INTERNAL "")



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


if(Neko_Profile)
    MESSAGE("Enable profiling")
    set(TRACY_FOLDER "${EXTERNAL_DIR}/tracy" CACHE INTERNAL "")
endif()

include("cmake/data_generation.cmake")
set(NEKO_CORE_DIR "${CMAKE_SOURCE_DIR}/core/" CACHE INTERNAL "")
add_subdirectory(${NEKO_CORE_DIR})

if(Neko_PhysFS)
    add_subdirectory(common/physfs_wrapper)
endif()



target_precompile_headers(Neko_Core PRIVATE "${NEKO_CORE_DIR}/include/core_pch.h")

if(Neko_SDL2)
    #sdl2
    add_subdirectory("common/sdl_engine")
endif()

if(Neko_Box2D)
    add_subdirectory("common/box2d_wrapper")
endif()

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
