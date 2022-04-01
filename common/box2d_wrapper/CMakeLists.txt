
find_package(box2d CONFIG REQUIRED)

file(GLOB_RECURSE b2_wrapper_src include/*.h src/*.cpp)
add_library(b2_wrapper STATIC ${b2_wrapper_src})

target_link_libraries(b2_wrapper PRIVATE box2d::box2d Neko_Core)
target_include_directories(b2_wrapper PUBLIC include/)

add_neko_proto(b2_wrapper)
set_target_properties (b2_wrapper PROPERTIES FOLDER Neko/Common)
neko_lib_config(b2_wrapper)
target_compile_options(b2_wrapper PRIVATE ${NEKO_COMPILE_FLAGS})
target_link_options(b2_wrapper PRIVATE ${NEKO_LINK_FLAGS})