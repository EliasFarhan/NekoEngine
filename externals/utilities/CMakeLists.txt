project(Utility CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./)

file(GLOB_RECURSE UTILITIES_SRC
        *.h *.hpp *.c *.cpp)

#add_library(UTILITY STATIC ${UTILITIES_SRC})
#set_property(TARGET UTILITY PROPERTY CXX_STANDARD 14)
