
function(add_data_folder binary)
    file(GLOB_RECURSE data_files "${CMAKE_CURRENT_SOURCE_DIR}/data/*")
    set(copy_data_name "${binary}_Copy_Data")
    foreach(DATA ${data_files})
        if(IS_DIRECTORY ${DATA})
            continue()
        endif()
        get_filename_component(FILE_NAME ${DATA} NAME)
        get_filename_component(PATH_NAME ${DATA} DIRECTORY)
        get_filename_component(EXTENSION ${DATA} EXT)
        file(RELATIVE_PATH PATH_NAME "${PROJECT_SOURCE_DIR}" ${PATH_NAME})
        set(DATA_OUTPUT "${PROJECT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}")
        add_custom_command(
                OUTPUT ${DATA_OUTPUT}
                DEPENDS ${DATA}
                DEPENDS
                COMMAND ${CMAKE_COMMAND} -E copy ${DATA} "${PROJECT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}"
        )
        list(APPEND DATA_BINARY_FILES ${DATA_OUTPUT})
    endforeach()

    add_custom_target(
            ${copy_data_name}
            DEPENDS ${DATA_BINARY_FILES} ${DATA_FILES})
    add_dependencies(${binary} ${copy_data_name})

endfunction()

