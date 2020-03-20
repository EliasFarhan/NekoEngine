MESSAGE("Hello ARM")
set(ARM ON CACHE BOOL "")
link_directories("/opt/vc/lib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --stdlib=libc++ -lc++abi")
