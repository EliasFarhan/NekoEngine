set(NEKO_WARNING_FLAGS "")
if(MSVC)
    set(NEKO_WARNING_FLAGS " /W4 ")
elseif(UNIX)
    set(NEKO_WARNING_FLAGS "-Wall -Wextra -Wswitch-default -Wmissing-declarations \
        -Wmissing-include-dirs -Wredundant-decls  -Wshadow -Wdouble-promotion \
        -Wundef -fno-common ")
endif()

function(neko_bin_config binary)
    set_target_properties(${binary} PROPERTIES UNITY_BUILD ON)
    set_property(TARGET ${binary} APPEND_STRING PROPERTY COMPILE_FLAGS " ${NEKO_WARNING_FLAGS} ")
endfunction()

function(neko_lib_config library)
    set_target_properties(${library} PROPERTIES UNITY_BUILD ON)
    set_property(TARGET ${library} APPEND_STRING PROPERTY COMPILE_FLAGS " ${NEKO_WARNING_FLAGS} ")
endfunction()
