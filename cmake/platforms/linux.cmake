

    MESSAGE("Linux Compiler ${CMAKE_CXX_COMPILER_ID}")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        MESSAGE("LLVM_ROOT ${LLVM_ROOT}")
        # using Clang

        if (LLVM_ROOT)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  --stdlib=libc++ -L${LLVM_ROOT}/lib -Wl,-rpath,${LLVM_ROOT}/lib  -lc++ -lc++abi ")#-fsanitize=thread
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -nostdinc++")

            include_directories(${LLVM_ROOT}/include/c++/v1/)
            if ("${CMAKE_CXX_COMPILER_VERSION}" STREQUAL "9.0.0")
            else ()
                LIST(APPEND Neko_LIBRARIES
                        c++fs)
            endif ()
            SET(CMAKE_AR "${LLVM_ROOT}/bin/llvm-ar")
            SET(CMAKE_LINKER "${LLVM_ROOT}/bin/llvm-ld")
            SET(CMAKE_NM "${LLVM_ROOT}/bin/llvm-nm")
            SET(CMAKE_OBJDUMP "${LLVM_ROOT}/bin/llvm-objdump")
            SET(CMAKE_RANLIB "${LLVM_ROOT}/bin/llvm-ranlib")
        else ()
            LIST(APPEND Neko_LIBRARIES
                    stdc++fs)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")# -fsanitize=address")
        endif ()

    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # using GCC
        LIST(APPEND Neko_LIBRARIES
                stdc++fs)
    endif ()