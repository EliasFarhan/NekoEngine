function(neko_bin_config binary)
    if(Emscripten)

        set_target_properties(${binary} PROPERTIES COMPILE_FLAGS_DEBUG
                " -s ASSERTIONS=1 -s DISABLE_EXCEPTION_CATCHING=0 -g4 -O0 -fno-rtti --js-opts -s ASSERTIONS=2 -s ALIASING_FUNCTION_POINTERS=0 -s SAFE_HEAP=2 -s DEMANGLE_SUPPORT=1 -msimd128 -s SIMD=1")
        set_target_properties(${binary} PROPERTIES COMPILE_FLAGS_RELEASE
                " -O3 -fno-rtti -fno-exceptions -msimd128 -s SIMD=1")
        set_target_properties(${binary} PROPERTIES LINK_FLAGS
                " -s EXPORT_ALL=1 --use-preload-plugins -s SIMD=1 -msimd128 --preload-file ${CMAKE_BINARY_DIR}/data@data/")
        set_target_properties(${binary} PROPERTIES SUFFIX ".html")
    elseif(MSVC)
            set_target_properties(${binary} PROPERTIES COMPILE_FLAGS " /arch:AVX2 /Oi ")
    elseif(UNIX)
        set_property(TARGET ${binary} APPEND_STRING PROPERTY COMPILE_FLAGS_DEBUG "-fno-rtti -fno-exceptions -Wall -Wextra")
        set_property(TARGET ${binary} APPEND_STRING PROPERTY COMPILE_FLAGS_RELEASE "-fno-rtti -fno-exceptions -Wall -Wextra")
        if(ARM)
            message("Adding ARM flags")
            set_property(TARGET ${binary} APPEND_STRING PROPERTY COMPILE_FLAGS "-march=native -mfpu=neon ")
        else()
            set_property(TARGET ${binary} APPEND_STRING PROPERTY COMPILE_FLAGS "-march=haswell ")

        endif()
    endif()
endfunction()

function(neko_lib_config library)
    #warning flags
    target_compile_options(${library} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            -Wall -Wextra>
            $<$<CXX_COMPILER_ID:MSVC>:
            /W4>)
    if(Emscripten)
        set_target_properties(${library} PROPERTIES COMPILE_FLAGS_DEBUG
                " -s ASSERTIONS=1 -s DISABLE_EXCEPTION_CATCHING=0 -g4 -O0 -fno-rtti --js-opts -msimd128 -s ASSERTIONS=2 -s ALIASING_FUNCTION_POINTERS=0 -s SAFE_HEAP=2 -s DEMANGLE_SUPPORT=1 -s SIMD=1")
        set_target_properties(${library} PROPERTIES COMPILE_FLAGS_RELEASE
                " -O3 -fno-rtti -fno-exceptions -s SIMD=1  -msimd128 ")
        set_target_properties(${library} PROPERTIES LINK_FLAGS " -s SIMD=1 -msimd128 -s EXPORT_ALL=1")    
    elseif(MSVC)
        set_target_properties(${library} PROPERTIES COMPILE_FLAGS " /arch:AVX2 /Oi ")
    elseif(UNIX)
        set_property(TARGET ${library} APPEND_STRING PROPERTY COMPILE_FLAGS_DEBUG "-fno-rtti -fno-exceptions -Wall -Wextra")
        set_property(TARGET ${library} APPEND_STRING PROPERTY COMPILE_FLAGS_RELEASE "-fno-rtti -fno-exceptions -Wall -Wextra")
        if(NOT Neko_Android)
        if(ARM)
            set_property(TARGET ${library} APPEND_STRING PROPERTY COMPILE_FLAGS "-march=native -mfpu=neon ")
        else()
            set_property(TARGET ${library} APPEND_STRING PROPERTY COMPILE_FLAGS "-march=haswell ")
        endif()
        endif()
    endif()
endfunction()
