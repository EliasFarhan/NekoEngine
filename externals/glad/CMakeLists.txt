file(GLOB_RECURSE glad_src include/*.h src/*.c)
add_library(glad STATIC ${glad_src})
set_target_properties (glad PROPERTIES FOLDER Externals)
target_include_directories(glad PUBLIC "include/")
