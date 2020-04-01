if(UNIX)
include_directories("${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/linux/api/core/lib/x86/" "${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/linux/api/studio/inc/x86/")

link_directories("${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/linux/api/core/lib/x86/" "${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/linux/api/studio/inc/x86/")

else()
include_directories("${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/win/api/core/lib/x86/" "${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/win/api/studio/inc/x86/")

link_directories("${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/win/api/core/lib/x86/" "${CMAKE_SOURCE_DIR}/externals/Fmod-2.00.08/win/api/studio/inc/x86/")

endif()
