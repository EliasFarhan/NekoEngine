cmake_minimum_required(VERSION 3.10)
project(Neko)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS On)

include(cmake/neko_executable.cmake)


set(Neko_SameThread OFF CACHE BOOL "Used for Emscripten and other system when render and engine has to be in the same thread")
set(Neko_Profile OFF CACHE BOOL "Activate Profiling with Easy Profile")
set(Neko_GLES3 ON CACHE BOOL "Activate OpenGL ES 3.0")
set(Neko_SDL2 ON CACHE BOOL "Activate SDL2")
set(Neko_Box2D OFF CACHE BOOL "Activate Box2D")
set(Neko_PhysFS ON CACHE BOOL "Activate PhysicsFS")
set(Neko_Test ON CACHE BOOL "Activate Test")
set(Neko_Assert ON CACHE BOOL "Activate Neko Assertion")
set(Neko_Benchmark ON CACHE BOOL "Activate Benchmark")

MESSAGE("CMAKE SYSTEM NAME: ${CMAKE_SYSTEM_NAME}")

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Emscripten")
    include(cmake/platforms/emscripten.cmake)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
    set(Neko_SameThread ON CACHE BOOL "Used for Emscripten and other system when render and engine has to be in the same thread")
    mark_as_advanced(Neko_SameThread)
    set(Neko_PhysFS OFF CACHE BOOL "Disable PhysFS for Emscripten.")
    mark_as_advanced(Neko_PhysFS)
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
    MESSAGE("Neko Update and Render will be on the same thread")
    add_compile_definitions("NEKO_SAMETHREAD=1")
endif()

if(Neko_Profile)
    set(EASY_PROFILE_DIR "${EXTERNAL_DIR}/easy_profiler-2.1.0")
    add_compile_definitions("EASY_PROFILE_USE=1")
    add_subdirectory(${EASY_PROFILE_DIR})
endif()

include("cmake/data.cmake")
set(NEKO_CORE_DIR "${CMAKE_SOURCE_DIR}/core/" CACHE INTERNAL "")
add_subdirectory(${NEKO_CORE_DIR})

#imgui
set(IMGUI_VERSION "1.74" CACHE INTERNAL "")
set(IMGUI_ROOT "${EXTERNAL_DIR}/imgui-${IMGUI_VERSION}" CACHE INTERNAL "")
add_subdirectory("${IMGUI_ROOT}")



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

endif()

if(Neko_Test)
    enable_testing()
    set(GOOGLE_TEST_DIR ${EXTERNAL_DIR}/googletest-1.8.1)
    set(BUILD_GMOCK OFF CACHE INTERNAL "")
    set(INSTALL_GTEST OFF CACHE INTERNAL "")
    add_subdirectory(${GOOGLE_TEST_DIR})
    include_directories(${GOOGLE_TEST_DIR}/include)

    set_target_properties (gtest gtest_main PROPERTIES
            FOLDER Externals\\GTest)
    add_subdirectory("test/")
endif()

if(Neko_Benchmark AND NOT Emscripten)
    #Google benchmark
    set(GOOGLE_BENCH_DIR "${EXTERNAL_DIR}/benchmark-1.5.0" CACHE STRING "")

    add_subdirectory(${GOOGLE_BENCH_DIR})
    set_target_properties (benchmark PROPERTIES FOLDER Externals)
    set_target_properties (benchmark_main PROPERTIES FOLDER Externals)
    add_subdirectory("benchmark/")

endif()

if(Neko_PhysFS)
    #physfs
    set(PHYSFS_VERSION "3.0.2" CACHE INTERNAL "")
    set(PHYSFS_ROOT "${EXTERNAL_DIR}/physfs-${PHYSFS_VERSION}" CACHE INTERNAL "")
    add_subdirectory("${PHYSFS_ROOT}")
    include_directories(${PHYSFS_ROOT}/src)

    add_subdirectory("common/physfs_wrapper")

endif()

set(main_project_dir "main/")
file(GLOB main_projects "${main_project_dir}/*")

foreach(main_project_path ${main_projects} )
    # I used a simple string replace, to cut off .cpp.
    get_filename_component(main_project_name ${main_project_path} NAME )

    message("Main project path: ${main_project_path} and name: ${main_project_name}")
    set(${main_project_name} OFF CACHE BOOL "")

    if(${main_project_name})
        add_subdirectory(${main_project_path})
    endif()
endforeach()
