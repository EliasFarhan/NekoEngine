cmake_minimum_required(VERSION 2.8.9)

# Define spine-sfml library
include_directories(src ${SFML_DIR}/include)
file(GLOB INCLUDES "src/**/*.h")
file(GLOB SOURCES "src/**/*.cpp")
add_library(spine-sfml STATIC ${SOURCES} ${INCLUDES})
target_link_libraries(spine-sfml LINK_PUBLIC spine-c)
