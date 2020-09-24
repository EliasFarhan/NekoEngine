
if(CMAKE_MINOR_VERSION LESS 11)
find_package(PythonInterp)
else()
find_package (Python3 REQUIRED COMPONENTS Interpreter)
endif()
MESSAGE("Python Interpreter: ${Python3_EXECUTABLE}")

file(GLOB_RECURSE SCRIPT_FILES scripts/*.py)


file(GLOB_RECURSE TEXT_FILES
        "${PROJECT_SOURCE_DIR}/data/*.json"
        "${PROJECT_SOURCE_DIR}/data/*.txt"
        "${PROJECT_SOURCE_DIR}/data/*.scene"
        )
file(GLOB_RECURSE MODEL_FILES
    "${PROJECT_SOURCE_DIR}/data/*.obj"
    "${PROJECT_SOURCE_DIR}/data/*.mtl"
    "${PROJECT_SOURCE_DIR}/data/*.fbx"
    )
file(GLOB_RECURSE SHADER_FILES
        "${PROJECT_SOURCE_DIR}/data/*.vert"
        "${PROJECT_SOURCE_DIR}/data/*.frag"
        )
file(GLOB_RECURSE IMG_FILES
        "${PROJECT_SOURCE_DIR}/data/*.jpg"
        "${PROJECT_SOURCE_DIR}/data/*.JPG"
        "${PROJECT_SOURCE_DIR}/data/*.png"
        "${PROJECT_SOURCE_DIR}/data/*.jpeg"
        "${PROJECT_SOURCE_DIR}/data/*.bmp"
        "${PROJECT_SOURCE_DIR}/data/*.tga"
        "${PROJECT_SOURCE_DIR}/data/*.gif"
        "${PROJECT_SOURCE_DIR}/data/*.psd"
        "${PROJECT_SOURCE_DIR}/data/*.hdr"
        "${PROJECT_SOURCE_DIR}/data/*.pic"
        "${PROJECT_SOURCE_DIR}/data/*.dds"
        "${PROJECT_SOURCE_DIR}/data/*.ktx"
        )
file(GLOB_RECURSE SND_FILES
        "${PROJECT_SOURCE_DIR}/data/*.wav"
        "${PROJECT_SOURCE_DIR}/data/*.ogg"
        )

file(GLOB_RECURSE FONT_FILES
        "${PROJECT_SOURCE_DIR}/data/*.ttf"
        )


source_group("Scripts"				FILES ${SCRIPT_FILES})
source_group("Data/Font"           FILES ${FONT_FILES})
source_group("Data/Text"           FILES ${TEXT_FILES})
source_group("Data/Img"            FILES ${IMG_FILES})
source_group("Data/Snd"			FILES ${SND_FILES})
source_group("Shaders"		FILES ${SHADER_FILES})
source_group("Data/Model" FILES ${MODEL_FILES})
LIST(APPEND DATA_FILES ${IMG_FILES} ${MODEL_FILES} ${SND_FILES} ${TEXT_FILES} ${SHADER_FILES} ${FONT_FILES})

foreach(DATA ${DATA_FILES})
    get_filename_component(FILE_NAME ${DATA} NAME)
    get_filename_component(PATH_NAME ${DATA} DIRECTORY)
    get_filename_component(EXTENSION ${DATA} EXT)
    file(RELATIVE_PATH PATH_NAME "${PROJECT_SOURCE_DIR}" ${PATH_NAME})
    set(DATA_OUTPUT "${PROJECT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}")

    add_custom_command(
            OUTPUT ${DATA_OUTPUT}
            DEPENDS ${DATA}
            DEPENDS 
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/validator/asset_validator.py"  "${DATA}" "${DATA_OUTPUT}"
            COMMAND ${CMAKE_COMMAND} -E copy ${DATA} "${PROJECT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}"
    )
    list(APPEND DATA_BINARY_FILES ${DATA_OUTPUT})
endforeach(DATA)

add_custom_target(
        DataTarget
        DEPENDS ${DATA_BINARY_FILES} ${DATA_FILES})

set_target_properties (DataTarget PROPERTIES FOLDER Neko/Core)

if(MSVC)
    if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
    else()
        set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
    endif()

elseif(UNIX)

    set(GLSL_VALIDATOR "glslangValidator")
endif()

find_program(GLSL_VALIDATOR_FOUND ${GLSL_VALIDATOR})
IF(NOT GLSL_VALIDATOR_FOUND)
    MESSAGE(FATAL_ERROR "Please install VulkanSDK and put it in path (current path: $ENV{VULKAN_SDK})")
ENDIF()