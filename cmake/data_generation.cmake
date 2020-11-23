find_package (Python3 REQUIRED COMPONENTS Interpreter)

MESSAGE("Python Interpreter: ${Python3_EXECUTABLE}")

file(GLOB_RECURSE SCRIPT_FILES scripts/*.py)
file(GLOB_RECURSE VALIDATOR_FILES validator/*.json)

source_group("Scripts"				FILES ${SCRIPT_FILES})
source_group("Validator"			FILES ${VALIDATOR_FILES})

function(data_generate binary)

    file(GLOB_RECURSE TEXT_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.json"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.txt"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.scene"
            )
    file(GLOB_RECURSE MODEL_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.obj"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.fbx"
            )
    file(GLOB_RECURSE SHADER_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.vert"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.frag"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.glsl"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.geom"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.comp"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.tesc"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.tese"
            )
    file(GLOB_RECURSE IMG_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.jpg"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.JPG"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.png"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.jpeg"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.bmp"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.tga"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.gif"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.psd"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.hdr"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.pic"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.dds"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.ktx"
            )
    file(GLOB_RECURSE SKY_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.skybox")
    file(GLOB_RECURSE SND_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.wav"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.ogg"
            )

    file(GLOB_RECURSE FONT_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.ttf"
            )
    file(GLOB_RECURSE MATERIAL_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.mat"
            "${CMAKE_CURRENT_SOURCE_DIR}/data/*.mtl")

    set(data_generate_name "${binary}_Generate_Data")

    source_group("Data/Font"           FILES ${FONT_FILES})
    source_group("Data/Text"           FILES ${TEXT_FILES})
    source_group("Data/Img"            FILES ${IMG_FILES})
    source_group("Data/Snd"			FILES ${SND_FILES})
    source_group("Data/Skybox"			FILES ${SKY_FILES})
    source_group("Shaders"		FILES ${SHADER_FILES})
    source_group("Data/Model" FILES ${MODEL_FILES})
    source_group("Data/Materials" FILES ${MATERIALS_FILES})

    LIST(APPEND DATA_FILES ${IMG_FILES} ${SKY_FILES} ${MODEL_FILES} ${SND_FILES} ${TEXT_FILES} ${SHADER_FILES} ${MATERIAL_FILES} ${FONT_FILES})

    foreach(DATA ${DATA_FILES})
        get_filename_component(FILE_NAME ${DATA} NAME)
        get_filename_component(PATH_NAME ${DATA} DIRECTORY)
        get_filename_component(EXTENSION ${DATA} EXT)
        file(RELATIVE_PATH PATH_NAME "${CMAKE_CURRENT_SOURCE_DIR}" ${PATH_NAME})
        set(DATA_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}")

        add_custom_command(
                OUTPUT ${DATA_OUTPUT}
                DEPENDS ${DATA}
                DEPENDS
                COMMAND ${CMAKE_COMMAND} -E copy ${DATA} "${CMAKE_CURRENT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}"
                COMMAND ${CMAKE_COMMAND} -E env VALIDATE_JSON_EXE=$<TARGET_FILE:validate_json> VALIDATOR_FOLDER=${CMAKE_SOURCE_DIR}/validator/
                BASISU_EXE=$<TARGET_FILE:basisu> SRC_FOLDER=${CMAKE_CURRENT_SOURCE_DIR} BINARY_FOLDER=${CMAKE_CURRENT_BINARY_DIR}
                IMAGE_FORMAT_EXE=$<TARGET_FILE:image_format>
                "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/validator/asset_validator.py"  "${DATA}" "${DATA_OUTPUT}"

        )
        list(APPEND DATA_BINARY_FILES ${DATA_OUTPUT})
    endforeach(DATA)

    add_custom_target(
            ${data_generate_name}
            DEPENDS ${DATA_BINARY_FILES} ${DATA_FILES})
    if(Neko_KTX)
        add_dependencies(${data_generate_name} basisu image_format)
    endif()
    add_dependencies(${data_generate_name} validate_json)
    add_dependencies(${binary} ${data_generate_name})
endfunction()
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