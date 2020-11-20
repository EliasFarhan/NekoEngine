
add_executable(image_format image_format.cpp)
target_include_directories(image_format PUBLIC ${UTILITIES_DIR})
set_target_properties (image_format PROPERTIES FOLDER Neko/Tools)