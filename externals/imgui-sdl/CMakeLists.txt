# Only tested in 3.1, may work in older versions.
# .
# Install SFML or set SFML_ROOT and set IMGUI_ROOT prior to running cmake
# .
# Provides the follow variables:
# IMGUI_SFML_INCLUDE_DIRS - imgui and imgui_sfml include paths
# IMGUI_SOURCES - imgui.cpp source path to link with your binary
# IMGUI_SFML_SOURCES - imgui_sfml.cpp source path to link with your binary
# IMGUI_SFML_DEPENDENCIES - found dependencies to link with your library (sfml)
# .
# Sample usage:
#   add_subdirectory(repos/imgui-sfml)
#   include_directories("${IMGUI_SFML_INCLUDE_DIRS}")
#   add_executable(MY_PROJECT ${IMGUI_SOURCES} ${IMGUI_SFML_SOURCES} ${SRCS})
#   ...
#   target_link_libraries(MY_PROJECT ${IMGUI_SFML_DEPENDENCIES})
# .
# Define spine-sfml library
if(IMGUI_ROOT)
    message("IMGUI ROOT: ${IMGUI_ROOT}")
	
	file(GLOB IMGUI_FILES
		"${IMGUI_ROOT}/imgui_draw.cpp"
		"${IMGUI_ROOT}/imgui_demo.cpp"
		"${IMGUI_ROOT}/imgui.cpp"
		"${IMGUI_ROOT}/imgui.h"
		"${IMGUI_ROOT}/imgui_widgets.cpp"
			)
	include_directories(${IMGUI_ROOT})
	# Rename that pesky imconfig.h file for the user.
	install(FILES ${IMGUI_INCLUDE_DIR}/imconfig.h DESTINATION include RENAME imconfig-sample.h)

endif()
file(GLOB_RECURSE INCLUDES "imgui_impl_opengl3.h" "imgui_impl_sdl.h" )
include_directories(${SDL_DIR}/include ${IMGUI}/include)
file(GLOB_RECURSE SOURCES  "imgui_impl_opengl3.cpp" "imgui_impl_sdl.cpp")
add_library(imgui-sdl STATIC ${SOURCES} ${INCLUDES} ${IMGUI_FILES})
target_link_libraries(imgui-sdl SDL2-static)

