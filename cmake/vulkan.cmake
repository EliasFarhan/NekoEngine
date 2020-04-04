function(compile_shader target_name shader_path)

add_custom_command(TARGET triangle POST_BUILD
    COMMAND ${GLSL_VALIDATOR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Compile Vulkan shader..."
)
add_custom_target(
    target_name ALL
    DEPENDS ${shader_path}
    ${output_path}
) 

endfunction()