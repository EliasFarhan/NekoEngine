
if(UNIX)
file(GLOB_RECURSE fmod_src "/linux/api/core/include/*.h" "/linux/api/core/src/*.cpp")
else()
file(GLOB_RECURSE fmod_src "/win/api/core/include/*.h" "/win/api/core/src/*.cpp")
endif()
add_library(fmod STATIC ${fmod_src})
if(UNIX)
include_directories("${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/linux/api/core/lib/x86/" "${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/linux/api/studio/inc/x86/")

link_directories("${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/linux/api/core/lib/x86/" "${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/linux/api/studio/inc/x86/")


target_link_libraries(fmod
    libfmod
    libfmodL
    libfmodstudio
    libfmodstudioL)

else()
include_directories("${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/win/api/core/lib/x86/" "${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/win/api/studio/inc/x86/")

link_directories("${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/win/api/core/lib/x86/" "${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/win/api/studio/inc/x86/")

target_link_libraries(fmod
        fmod_vc
        fmodL_vc
		fmodstudio_vc
    	fmodstudioL_vc)

endif()
