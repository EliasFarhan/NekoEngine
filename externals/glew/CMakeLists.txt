cmake_minimum_required(VERSION 3.10)
project(glew)

include_directories(include/)
add_library(glew src/glew.c include/GL/glew.h)
