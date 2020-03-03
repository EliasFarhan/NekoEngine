cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE target "include/*.h" "src/*.cpp")

if(UNIX)
include_directories(NekoEngine/externals/Fmod-2.00.08/linux/api/core/inc/x86/ NekoEngine/externals/Fmod-2.00.08/linux/api/studio/inc/x86/)

link_directories("NekoEngine/externals/Fmod-2.00.08/linux/api/core/lib/x86/" "NekoEngine/externals/Fmod-2.00.08/linux/api/studio/lib/x86/")

target_link_libraries(${target}
	libfmod
	libfmodL
	libfmodstudio
	libfmodstudioL)

else()
include_directories(NekoEngine/externals/Fmod-2.00.08/win/api/core/inc/x86/ NekoEngine/externals/Fmod-2.00.08/win/api/studio/inc/x86/)

link_directories("NekoEngine/externals/Fmod-2.00.08/win/api/core/lib/x86/" "NekoEngine/externals/Fmod-2.00.08/win/api/studio/lib/x86/")

target_link_libraries(${target}
        fmod_vc
    	fmodL_vc
	fmodstudio_vc
	fmodstudioL_vc)

endif()