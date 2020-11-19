
add_executable(validate_json validate_json.cpp)
target_include_directories(validate_json PUBLIC ${UTILITIES_DIR})
set_target_properties (validate_json PROPERTIES FOLDER Neko/Tools)