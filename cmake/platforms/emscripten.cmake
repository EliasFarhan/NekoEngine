set(Emscripten ON CACHE BOOL "")

add_compile_definitions(EMSCRIPTEN=1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -s ASSERTIONS=1 -g4 -O0 " CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --emrun -s EXPORT_ALL=1 --use-preload-plugins --preload-file shaders/ " CACHE STRING "")

set(NEKO_LIBS_FLAGS "")

set(CMAKE_EXECUTABLE_SUFFIX ".html")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")

set(Neko_Benchmark BOOL OFF CACHE FORCE BOOL "")