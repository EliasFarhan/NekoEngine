
file(GLOB_RECURSE ft_wrapper_src include/*.h src/*.cpp)
LIST(APPEND GLES3_WRAPPER_LIBS Neko_Core)

add_library(ft_wrapper STATIC ${ft_wrapper_src})
target_include_directories(ft_wrapper PUBLIC include/)
target_link_libraries(ft_wrapper PUBLIC gles3_wrapper freetype)
set_target_properties (ft_wrapper PROPERTIES FOLDER Neko/Common)