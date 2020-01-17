cmake_minimum_required(VERSION 3.10)
project(Neko)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS On)

include(cmake/neko_executable.cmake)

set(Neko_Remotery FALSE CACHE INTERNAL "Activate Remotery")
set(Neko_GLES3 TRUE CACHE INTERNAL "Activate OpenGL ES 3.0")
set(Neko_SDL2 TRUE CACHE INTERNAL "Activate SDL2")
set(Neko_Box2D FALSE CACHE INTERNAL "Activate Box2D")
set(Neko_Test FALSE CACHE INTERNAL "Activate Test")
set(Neko_Benchmark FALSE CACHE INTERNAL "Activate Benchmark")


set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/externals CACHE INTERNAL "Externals folder")

#imgui
set(IMGUI_VERSION "1.74" CACHE INTERNAL "")
set(IMGUI_ROOT "${EXTERNAL_DIR}/imgui-${IMGUI_VERSION}" CACHE INTERNAL "")
add_subdirectory("${IMGUI_ROOT}")

if(Neko_Remotery)
    #Remotery
    set(REMOTERY_DIR ${EXTERNAL_DIR}/Remotery)
    add_subdirectory(${REMOTERY_DIR})
    include_directories(${REMOTERY_DIR})
    LIST(APPEND Neko_LIBRARIES Remotery)
    set_target_properties (Remotery PROPERTIES FOLDER Externals)
endif()

if(Neko_SDL2)
    #sdl2
    set(SDL_VERSION 2.0.9)
    set(SDL_DIR ${EXTERNAL_DIR}/SDL2-${SDL_VERSION}/)
    set(SDL_STATIC OFF CACHE INTERNAL "")
    set(SDL_SHARED ON CACHE INTERNAL "")

    add_subdirectory(${SDL_DIR})
    include_directories(${SDL_DIR}/include ${SDL_DIR}/src)
    add_subdirectory("common/sdl_engine")
endif()

if(Neko_GLES3)
    add_compile_definitions("NEKO_GLES=1")
endif()

add_subdirectory("data/")
add_subdirectory("core/")



