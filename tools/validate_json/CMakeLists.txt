find_package(nlohmann_json CONFIG REQUIRED)

add_executable(validate_json validate_json.cpp)
target_link_libraries(validate_json PUBLIC nlohmann_json nlohmann_json::nlohmann_json)
target_include_directories(validate_json PUBLIC ${UTILITIES_DIR})
set_target_properties (validate_json PROPERTIES FOLDER Neko/Tools)