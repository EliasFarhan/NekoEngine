# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.



# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )
find_library(opengl-lib
        GLESv3)

set(NEKO_SRC_DIR "${CMAKE_SOURCE_DIR}/../../../../../../")
set(EXTERNAL_DIR "${NEKO_SRC_DIR}/externals/")

add_definitions(-DXXH_FORCE_MEMORY_ACCESS=0)
add_definitions(-DNEKO_GLES3=1)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE INTERNAL "")
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE INTERNAL "")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE INTERNAL "")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "")
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")
set(ASSIMP_INSTALL OFF CACHE INTERNAL "")

SET (ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
SET (ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
SET (ASSIMP_BUILD_BLEND_IMPORTER ON CACHE BOOL "" FORCE)
SET (ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)

include_directories(${NEKO_SRC_DIR}/externals/utilities)
include_directories(${NEKO_SRC_DIR}/externals/imgui-1.74)
#SDL_ENGINE
set(Neko_Profile ON CACHE BOOL "Activate easy profiler")
if(Neko_Profile)
    set(EASY_PROFILE_DIR "${EXTERNAL_DIR}/easy_profiler-2.1.0")
    set(EASY_OPTION_LOG ON CACHE BOOL "Print errors to stderr" FORCE)
    add_definitions(-DEASY_PROFILE_USE=1)
    add_subdirectory(${EASY_PROFILE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/easy_profiler)
endif()
set(Neko_GLES3 ON CACHE BOOL "Activate OpenGL ES 3.0")
set(Neko_SDL2 ON CACHE BOOL "Activate SDL2")
set(Neko_Android ON CACHE BOOL "Android port")
set(EXTERNAL_DIR "${NEKO_SRC_DIR}/externals" CACHE STRING "")

MESSAGE("CMAKE MODULE PATH: ${CMAKE_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

add_subdirectory(${NEKO_SRC_DIR}/externals/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
add_subdirectory(${NEKO_SRC_DIR}/externals/imgui-1.74 ${CMAKE_CURRENT_BINARY_DIR}/imgui-1.74)
add_subdirectory(${NEKO_SRC_DIR}/externals/SDL2-2.0.9 ${CMAKE_CURRENT_BINARY_DIR}/SDL2-2.0.9)
add_subdirectory(${NEKO_SRC_DIR}/externals/assimp-5.0.1 ${CMAKE_CURRENT_BINARY_DIR}/assimp-5.0.1)
add_subdirectory("${NEKO_SRC_DIR}/externals/freetype-2.10.0" ${CMAKE_CURRENT_BINARY_DIR}/freetype-2.10.0)

include(${NEKO_SRC_DIR}/cmake/data.cmake)
include(${NEKO_SRC_DIR}/cmake/neko_executable.cmake)
add_subdirectory(${NEKO_SRC_DIR}/core ${CMAKE_CURRENT_BINARY_DIR}/core)
add_subdirectory(${NEKO_SRC_DIR}/common/sdl_engine ${CMAKE_CURRENT_BINARY_DIR}/sdl_engine)
add_subdirectory(${NEKO_SRC_DIR}/common/gles3_wrapper ${CMAKE_CURRENT_BINARY_DIR}/gles3_wrapper)
add_subdirectory(${NEKO_SRC_DIR}/common/assimp_wrapper ${CMAKE_CURRENT_BINARY_DIR}/assimp_wrapper)
add_subdirectory(${NEKO_SRC_DIR}/common/ft_wrapper ${CMAKE_CURRENT_BINARY_DIR}/ft_wrapper)

#comp_graph
set(COMP_GRAPH_DIR ${NEKO_SRC_DIR}/main/comp_graph)
include_directories( ${COMP_GRAPH_DIR}/include)
file(GLOB_RECURSE comp_graph_src ${COMP_GRAPH_DIR}/*.cpp)
add_library( # Sets the name of the library.
        main
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        ${comp_graph_src})
# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       main

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} ${opengl-lib} Neko_Core SDL2-static sdl_engine glad gles3_wrapper imgui assimp_wrapper ft_wrapper)