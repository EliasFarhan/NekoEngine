function(add_neko_executable binary source_file)

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
    target_compile_options(${binary} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall -Wextra>
        $<$<CXX_COMPILER_ID:MSVC>:
        /W4>)
    target_link_libraries(${binary} PUBLIC ${ARGN})
    set_property(TARGET ${binary} PROPERTY CXX_STANDARD 17)
    set_target_properties (${binary} PROPERTIES FOLDER Main)
    add_dependencies(${binary} DataTarget)
endfunction()

function(add_neko_application binary source_folder)
    file(GLOB_RECURSE app_source ${source_folder}/*.cpp ${source_folder}/*.h)
    #city Builder
    if(APPLE)
        add_executable(${binary} MACOSX_BUNDLE ${app_source})
        add_custom_command(
                TARGET ${binary}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory data ${CMAKE_BINARY_DIR}/${binary}.app/Contents/MacOS/data
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${SFML_DIR}/extlibs/libs-osx/Frameworks/ ${CMAKE_BINARY_DIR}/${binary}.app/Contents/Frameworks
        )
    else()
        add_executable(${binary} ${app_source})
    endif()
    target_compile_options(${binary} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            -Wall -Wextra>
            $<$<CXX_COMPILER_ID:MSVC>:
            /W4>)
    target_link_libraries(${binary} PUBLIC ${ARGN})
    include_directories(${source_folder}/include)
    set_property(TARGET ${binary} PROPERTY CXX_STANDARD 17)
    set_target_properties (${binary} PROPERTIES FOLDER Main)
    add_dependencies(${binary} DataTarget)
endfunction()

function(add_neko_library binary source_folder)
    include_directories(${source_folder}/include)
    file(GLOB_RECURSE LIB_FILES ${source_folder}/*.cpp ${source_folder}/*.h)
    add_library(${binary} STATIC ${LIB_FILES})

    target_compile_options(${binary} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            >
            $<$<CXX_COMPILER_ID:MSVC>:
            /W4>)
    target_link_libraries(${binary} PUBLIC ${ARGN})
    set_property(TARGET ${binary} PROPERTY CXX_STANDARD 17)
    set_target_properties (${binary} PROPERTIES FOLDER Main)
    add_dependencies(${binary} DataTarget)
endfunction()

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
        if(ARM)
            set_property(TARGET ${library} APPEND_STRING PROPERTY COMPILE_FLAGS "-march=native -mfpu=neon ")
        else()
            set_property(TARGET ${library} APPEND_STRING PROPERTY COMPILE_FLAGS "-march=haswell ")
        endif()
    endif()
endfunction()
