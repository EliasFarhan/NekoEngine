include_directories("include/")

file(GLOB_RECURSE glad_src include/*.h src/*.c)
add_library(glad STATIC ${glad_src})
