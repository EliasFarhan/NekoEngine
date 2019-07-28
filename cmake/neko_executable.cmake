function(add_neko_executable binary source_file)
    #City Builder
    if(APPLE)
        add_executable(${binary} MACOSX_BUNDLE ${source_file})
        add_custom_command(
                TARGET ${binary}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory data ${CMAKE_BINARY_DIR}/${binary}.app/Contents/MacOS/data
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SFML_DIR}/extlibs/libs-osx/Frameworks/ ${CMAKE_BINARY_DIR}/${binary}.app/Contents/Frameworks
        )
    else()
        add_executable(${binary} ${source_file})
    endif()
    target_link_libraries(${binary} PUBLIC ${ARGN})
    set_property(TARGET ${binary} PROPERTY CXX_STANDARD 17)
    set_target_properties (${binary} PROPERTIES FOLDER Main)
    add_dependencies(${binary} DataTarget)
endfunction()