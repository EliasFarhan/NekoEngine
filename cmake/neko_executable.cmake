function(neko_bin_config binary)

    if(Emscripten)
        if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
            set_target_properties(${binary} PROPERTIES  COMPILE_FLAGS
                " -s USE_PTHREADS=1  -g4 -O0 -fno-rtti -s ASSERTIONS=2 -s ALIASING_FUNCTION_POINTERS=1 -s SAFE_HEAP=1 -s DEMANGLE_SUPPORT=1  ")#-s SIMD=1
        endif()
        if("${CMAKE_BUILD_TYPE}" MATCHES "Release")
        set_target_properties(${binary} PROPERTIES   COMPILE_FLAGS
                " -s USE_PTHREADS=1 -O3 -fno-rtti -fno-exceptions -s ASSERTIONS=1 ")
        endif()
        set_target_properties(${binary} PROPERTIES  LINK_FLAGS
                " -s PTHREAD_POOL_SIZE=4 -Wl,--shared-memory,--no-check-features -s ASSERTIONS=1 -s EXPORT_ALL=1 -s TOTAL_MEMORY=128MB --preload-file ${CMAKE_BINARY_DIR}/data@data/ ")
        set_target_properties(${binary} PROPERTIES SUFFIX ".html")
    elseif(MSVC)
            set_target_properties(${binary} PROPERTIES COMPILE_FLAGS " /arch:AVX2 /Oi ")
    elseif(UNIX)
        set_property(TARGET ${binary} APPEND_STRING PROPERTY COMPILE_FLAGS_DEBUG " -fno-rtti -fno-exceptions -Wall -Wextra ")
        set_property(TARGET ${binary} APPEND_STRING PROPERTY COMPILE_FLAGS_RELEASE " -fno-rtti -fno-exceptions -Wall -Wextra ")
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
    set_target_properties(${library} PROPERTIES UNITY_BUILD ON)
    if(Emscripten)
        if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
            set_target_properties(${library} PROPERTIES  COMPILE_FLAGS
                " -s USE_PTHREADS=1  -g4 -O0 -fno-rtti -s ASSERTIONS=2 -s ALIASING_FUNCTION_POINTERS=1 -s SAFE_HEAP=1 -s DEMANGLE_SUPPORT=1  ") #-s SIMD=1
        endif()
        if("${CMAKE_BUILD_TYPE}" MATCHES "Release")
            set_target_properties(${library} PROPERTIES  COMPILE_FLAGS
                " -s USE_PTHREADS=1 -O3 -fno-rtti -fno-exceptions -s ASSERTIONS=1 ")
        endif()
        set_property(TARGET ${library} APPEND_STRING PROPERTY LINK_FLAGS
                " -s PTHREAD_POOL_SIZE=4  -s EXPORT_ALL=1 -s TOTAL_MEMORY=128MB -s ASSERTIONS=1 ")
        #-Wl,--shared-memory,--no-check-features
    elseif(MSVC)
        set_target_properties(${library} PROPERTIES COMPILE_FLAGS " /arch:AVX2 /Oi ")
    elseif(UNIX)
        set_property(TARGET ${library} APPEND_STRING PROPERTY CMAKE_CXX_FLAGS_DEBUG "-fno-rtti -fno-exceptions -Wall -Wextra")
        set_property(TARGET ${library} APPEND_STRING PROPERTY CMAKE_CXX_FLAGS_RELEASE "-fno-rtti -fno-exceptions -Wall -Wextra")
        if(NOT Neko_Android)
            if(ARM)
                set_property(TARGET ${library} APPEND_STRING PROPERTY COMPILE_FLAGS "-march=native -mfpu=neon ")
            else()
                set_property(TARGET ${library} APPEND_STRING PROPERTY COMPILE_FLAGS "-march=haswell ")
            endif()
        endif()
    endif()
endfunction()
