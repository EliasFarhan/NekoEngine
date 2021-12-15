find_package(fmt CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
#source files
file(GLOB_RECURSE Neko_ENGINE_SRC src/engine/*.cpp include/engine/*.h)
file(GLOB_RECURSE Neko_GRAPHICS_SRC src/graphics/*.cpp include/graphics/*.h)
file(GLOB_RECURSE Neko_UTILITY_SRC src/utils/*.cpp include/utils/*.h)
file(GLOB_RECURSE Neko_PHYSICS_SRC src/physics/*.cpp include/physics/*.h)
file(GLOB_RECURSE Neko_SOUND_SRC src/sound/*.cpp include/sound/*.h)
file(GLOB_RECURSE Neko_MATH_SRC src/mathematics/*.cpp include/mathematics/*.h)

source_group("Graphics"				FILES ${Neko_GRAPHICS_SRC})
source_group("Engine"				FILES ${Neko_ENGINE_SRC})
source_group("Utility"				FILES ${Neko_UTILITY_SRC})
source_group("Physics"				FILES ${Neko_PHYSICS_SRC})
source_group("Sound"				FILES ${Neko_SOUND_SRC})
source_group("Maths"			FILES ${Neko_MATH_SRC})

List(APPEND Neko_SRC
        ${Neko_ENGINE_SRC}
        ${Neko_GRAPHICS_SRC}
        ${Neko_UTILITY_SRC}
        ${Neko_PHYSICS_SRC}
        ${Neko_SOUND_SRC}
        ${Neko_MATH_SRC})

#Utility
set(UTILITIES_DIR ${EXTERNAL_DIR}/utilities/)
if(Neko_Profile)
    list(APPEND Neko_SRC "${TRACY_FOLDER}/TracyClient.cpp" "${TRACY_FOLDER}/Tracy.hpp")
endif()
add_library(Neko_Core STATIC ${Neko_SRC} include/core_pch.h)
target_include_directories(Neko_Core PUBLIC "${IMGUI_ROOT}")
target_link_libraries(Neko_Core PUBLIC imgui::imgui fmt::fmt-header-only nlohmann_json nlohmann_json::nlohmann_json spdlog::spdlog_header_only)

if(Neko_Assert)
    target_compile_definitions(Neko_Core PUBLIC "NEKO_ASSERT=1")
endif()
if(Neko_Profile)
    target_compile_definitions(Neko_Core PUBLIC "TRACY_ENABLE=1")
    target_include_directories(Neko_Core PUBLIC "${TRACY_FOLDER}")
endif()

target_compile_options(Neko_Core PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall -Wextra>
        $<$<CXX_COMPILER_ID:MSVC>:
        /W4>)

if(MSVC)
    target_compile_definitions(Neko_Core PUBLIC WIN32_LEAN_AND_MEAN NOMINMAX)
endif()
target_compile_definitions(Neko_Core PUBLIC UNIT_LIB_DISABLE_IOSTREAM DISABLE_PREDEFINED_UNITS ENABLE_PREDEFINED_ANGLE_UNITS UNIT_LIB_DEFAULT_TYPE=float)
target_compile_definitions(Neko_Core PUBLIC $<$<CONFIG:Debug>:__neko_dbg__>)
target_compile_definitions(Neko_Core PUBLIC $<$<CONFIG:RelWithDebInfo>:__neko_dbg__>)
target_compile_definitions(Neko_Core PUBLIC SOURCE_PATH="${CMAKE_SOURCE_DIR}")

set_target_properties (Neko_Core PROPERTIES FOLDER Neko/Core)

target_include_directories(Neko_Core PUBLIC "${IMGUI_ROOT}")
target_include_directories(Neko_Core PUBLIC "include/")
target_include_directories(Neko_Core PUBLIC "${UTILITIES_DIR}")
target_precompile_headers(Neko_Core PUBLIC include/core_pch.h)


neko_lib_config(Neko_Core)
add_neko_proto(Neko_Core)
if(Neko_Test)
    add_neko_test(Neko_Core)
endif()
