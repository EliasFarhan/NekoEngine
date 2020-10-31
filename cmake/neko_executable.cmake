set(COMPILER_WARNING_FLAGS "")
if(UNIX)
set(COMPILER_WARNING_FLAGS "-Wall -Wextra -Wswitch-default -Wmissing-declarations \
-Wmissing-include-dirs -Wredundant-decls  -Wshadow -Wdouble-promotion \
-Wundef -fno-common ")
endif()

function(neko_bin_config binary)

    if(Emscripten)
        if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
            set_target_properties(${binary} PROPERTIES  COMPILE_FLAGS
                "   -g4 -O0 -fno-rtti -s ASSERTIONS=2 -s ALIASING_FUNCTION_POINTERS=1 -s SAFE_HEAP=1 -s DEMANGLE_SUPPORT=1  ")#-s SIMD=1
        endif()
        if("${CMAKE_BUILD_TYPE}" MATCHES "Release")
        set_target_properties(${binary} PROPERTIES   COMPILE_FLAGS
                "  -O3 -fno-rtti -fno-exceptions -s ASSERTIONS=1 ")
        endif()
        set_target_properties(${binary} PROPERTIES  LINK_FLAGS
                " -flto  --emrun -s ASSERTIONS=1 -s EXPORT_ALL=1 -s TOTAL_MEMORY=512MB ")

    elseif(MSVC)
            set_target_properties(${binary} PROPERTIES COMPILE_FLAGS " /arch:AVX2 /Oi /GR- ")
    elseif(UNIX)
        if(ARM)
            message("Adding ARM flags to ${binary}")
            set_property(TARGET ${binary} APPEND_STRING PROPERTY COMPILE_FLAGS " -fno-rtti -fno-exceptions ${COMPILER_WARNING_FLAGS} ")
        else()
            set_property(TARGET ${binary} APPEND_STRING PROPERTY COMPILE_FLAGS "-march=haswell -fno-rtti -fno-exceptions ${COMPILER_WARNING_FLAGS} ")

        endif()
    endif()
endfunction()

function(neko_lib_config library)
    #warning flags
    target_compile_options(${library} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            -Wall -Wextra>
            $<$<CXX_COMPILER_ID:MSVC>:
            /W4>)
    set_target_properties(${library} PROPERTIES UNITY_BUILD ON)
    if(Emscripten)
        MESSAGE("Adding Emscripten flags to ${library}")
        if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
            set_target_properties(${library} PROPERTIES  COMPILE_FLAGS
                "  -g4 -O0 -fno-rtti -s ASSERTIONS=2 -s ALIASING_FUNCTION_POINTERS=1 -s SAFE_HEAP=1 -s DEMANGLE_SUPPORT=1 -s USE_WEBGL2=1 ") #-s SIMD=1
        endif()
        if("${CMAKE_BUILD_TYPE}" MATCHES "Release")
            set_target_properties(${library} PROPERTIES  COMPILE_FLAGS
                "  -O3 -fno-rtti -fno-exceptions -s ASSERTIONS=1 -s USE_WEBGL2=1 ")
        endif()
        set_property(TARGET ${library} APPEND_STRING PROPERTY LINK_FLAGS
                " -flto  -s EXPORT_ALL=1 -s TOTAL_MEMORY=512MB -s ASSERTIONS=1 --emrun ")
        #-Wl,--shared-memory,--no-check-features
    elseif(MSVC)
        set_target_properties(${library} PROPERTIES COMPILE_FLAGS " /arch:AVX2 /Oi /GR-")
    elseif(UNIX)
        if(NOT Neko_Android)
            if(ARM)
                message("Adding ARM flags to ${library}")
                set_property(TARGET ${library} APPEND_STRING PROPERTY COMPILE_FLAGS " -fno-rtti -fno-exceptions ${COMPILER_WARNING_FLAGS} ")
            else()
                set_property(TARGET ${library} APPEND_STRING PROPERTY COMPILE_FLAGS "-march=haswell -fno-rtti -fno-exceptions ${COMPILER_WARNING_FLAGS} ")
            endif()
        endif()
    endif()
endfunction()
