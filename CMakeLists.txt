cmake_minimum_required(VERSION 3.10)
project(Neko)

if(WIN32)
add_compile_definitions(NOMINMAX)
endif(WIN32)


set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
		$<$<CONFIG:RelWithDebInfo>:/MT>
    )
	if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
		set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
	else()
		set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
	endif()
elseif(UNIX)
	set(GLSL_VALIDATOR "glslangValidator")
endif()

find_program(GLSL_VALIDATOR_FOUND ${GLSL_VALIDATOR})
IF(NOT GLSL_VALIDATOR_FOUND)
	MESSAGE(FATAL_ERROR "Please install VulkanSDK and put it in path (current path: $ENV{VULKAN_SDK})")
ENDIF()

if (APPLE)
	find_package(Boost 1.66.0 REQUIRED COMPONENTS
	  program_options
	  filesystem
	  system
	  date_time
	  chrono
	)

	if(NOT Boost_FOUND)
	  message(FATAL_ERROR "Boost not found")
	endif()

	include_directories(${Boost_INCLUDE_DIRS})
	LIST(APPEND Neko_LIBRARIES ${Boost_LIBRARIES})
endif(APPLE)

if (UNIX)
	find_package(Freetype REQUIRED)
	include_directories(${FREETYPE_INCLUDE_DIRS})
endif(UNIX)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/externals)



#data

file(GLOB_RECURSE TEXT_FILES 
"${PROJECT_SOURCE_DIR}/data/*.json" 
"${PROJECT_SOURCE_DIR}/data/*.txt"
"${PROJECT_SOURCE_DIR}/data/*.scene"
)
file(GLOB_RECURSE SHADER_FILES
"${PROJECT_SOURCE_DIR}/data/*.vert"
"${PROJECT_SOURCE_DIR}/data/*.frag"
)
file(GLOB_RECURSE IMG_FILES
		"${PROJECT_SOURCE_DIR}/data/*.jpg"
		"${PROJECT_SOURCE_DIR}/data/*.png"
		"${PROJECT_SOURCE_DIR}/data/*.jpeg"
		"${PROJECT_SOURCE_DIR}/data/*.bmp"
		"${PROJECT_SOURCE_DIR}/data/*.tga"
		"${PROJECT_SOURCE_DIR}/data/*.gif"
		"${PROJECT_SOURCE_DIR}/data/*.psd"
		"${PROJECT_SOURCE_DIR}/data/*.hdr"
		"${PROJECT_SOURCE_DIR}/data/*.pic"
		)
file(GLOB_RECURSE SND_FILES
		"${PROJECT_SOURCE_DIR}/data/*.wav"
		"${PROJECT_SOURCE_DIR}/data/*.ogg"
		)
find_package (Python3 COMPONENTS Interpreter)
MESSAGE("Python Interpreter: ${Python3_EXECUTABLE}")
LIST(APPEND DATA_FILES ${IMG_FILES} ${SND_FILES} ${TEXT_FILES} ${SHADER_FILES})
foreach(DATA ${DATA_FILES})
	get_filename_component(FILE_NAME ${DATA} NAME)
	get_filename_component(PATH_NAME ${DATA} DIRECTORY)
	get_filename_component(EXTENSION ${DATA} EXT)
	file(RELATIVE_PATH PATH_NAME "${PROJECT_SOURCE_DIR}" ${PATH_NAME})
	MESSAGE("DATA: ${FILE_NAME} ${PATH_NAME} ${EXTENSION}")
	set(DATA_OUTPUT "${PROJECT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}")
	if(${EXTENSION} STREQUAL ".scene")
		add_custom_command(
				OUTPUT ${DATA_OUTPUT}
				DEPENDS ${DATA}
				COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/${PATH_NAME}"
				COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/scene_validator.py"  "${DATA}"
				COMMAND ${CMAKE_COMMAND} -E copy ${DATA} "${PROJECT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}")
	elseif(${EXTENSION} STREQUAL ".atlas.txt")
		add_custom_command(
				OUTPUT ${DATA_OUTPUT}
				DEPENDS ${DATA} "${CMAKE_SOURCE_DIR}/scripts/spine_validator.py"
				COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/${PATH_NAME}"
				COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/spine_validator.py"  "${DATA}" "${PROJECT_SOURCE_DIR}/${PATH_NAME}/" "${PROJECT_BINARY_DIR}/${PATH_NAME}/"
				COMMAND ${CMAKE_COMMAND} -E copy ${DATA} "${PROJECT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}")
	elseif(${EXTENSION} STREQUAL ".vert" OR ${EXTENSION} STREQUAL ".frag")
		add_custom_command(
				OUTPUT ${DATA_OUTPUT}
				DEPENDS ${DATA}
				COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/${PATH_NAME}"
				COMMAND "${GLSL_VALIDATOR}" "${DATA}" 
				COMMAND ${CMAKE_COMMAND} -E copy ${DATA} "${PROJECT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}")
	else()
	add_custom_command(
			OUTPUT ${DATA_OUTPUT}
			DEPENDS ${DATA}
			COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/${PATH_NAME}"
			COMMAND ${CMAKE_COMMAND} -E copy ${DATA} "${PROJECT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}")
	endif()
	list(APPEND DATA_BINARY_FILES ${DATA_OUTPUT})
endforeach(DATA)
add_custom_target(
		DataTarget
		DEPENDS ${DATA_BINARY_FILES})

set_target_properties (DataTarget PROPERTIES FOLDER Neko)

#source files
file(GLOB_RECURSE Neko_ENGINE_SRC src/engine/*.cpp include/engine/*.h)
file(GLOB_RECURSE Neko_GRAPHICS_SRC src/graphics/*.cpp include/graphics/*.h)
file(GLOB_RECURSE Neko_UTILITY_SRC src/utilities/*.cpp include/utilities/*.h)
file(GLOB_RECURSE Neko_PHYSICS_SRC src/physics/*.cpp include/physics/*.h)
file(GLOB_RECURSE Neko_SOUND_SRC src/sound/*.cpp include/sound/*.h)
file(GLOB_RECURSE Neko_MATH_SRC src/mathematics/*.cpp include/mathematics/*.h)
file(GLOB_RECURSE Neko_EXTENSIONS_SRC extensions/src/*.cpp extensions/include/*.h)
file(GLOB_RECURSE SCRIPT_FILES scripts/*.py)

include_directories(include ${CMAKE_SOURCE_DIR}/include)
set_property(GLOBAL PROPERTY USE_FOLDERS On)
SET_SOURCE_FILES_PROPERTIES( ${Neko_SRC} PROPERTIES
                FOLDER "${CMAKE_SOURCE_DIR}/src" )

source_group("Graphics"				FILES ${Neko_GRAPHICS_SRC})
source_group("Engine"				FILES ${Neko_ENGINE_SRC})
source_group("Utility"				FILES ${Neko_UTILITY_SRC})
source_group("Physics"				FILES ${Neko_PHYSICS_SRC})
source_group("Sound"				FILES ${Neko_SOUND_SRC})
source_group("Maths"			FILES ${Neko_MATH_SRC})
source_group("Data\\Text"           FILES ${TEXT_FILES})
source_group("Data\\Img"            FILES ${IMG_FILES})
source_group("Data\\Snd"			FILES ${SND_FILES})
source_group("Data\\Shaders"		FILES ${SHADER_FILES})
source_group("Scripts"				FILES ${SCRIPT_FILES})

List(APPEND Neko_SRC 
	${Neko_ENGINE_SRC} 
	${Neko_GRAPHICS_SRC} 
	${Neko_UTILITY_SRC} 
	${Neko_PHYSICS_SRC}
	${Neko_SOUND_SRC}
	${Neko_MATH_SRC}
	${TEXT_FILES} 
	${IMG_FILES}
	${SND_FILES}
	${SHADER_FILES}
	${SCRIPT_FILES})

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:RelWithDebInfo>:/MTd>
		$<$<CONFIG:Release>:/MT> #--|
    )
endif()



#Google Test
enable_testing()
set(GOOGLE_TEST_DIR ${EXTERNAL_DIR}/googletest-1.8.1)
set(BUILD_GMOCK OFF CACHE INTERNAL "")
set(INSTALL_GTEST OFF CACHE INTERNAL "")
add_subdirectory(${GOOGLE_TEST_DIR})
include_directories(${GOOGLE_TEST_DIR}/include)

set_target_properties (gtest gtest_main PROPERTIES
		FOLDER Externals\\GTest)
#Google benchmark
set(GOOGLE_BENCH_DIR ${EXTERNAL_DIR}/benchmark-1.5.0)
add_subdirectory(${GOOGLE_BENCH_DIR})
include_directories(${GOOGLE_BENCH_DIR}/include)
set_target_properties (benchmark benchmark_main PROPERTIES
		FOLDER Externals\\GTest)
#sdl2 
set(SDL_VERSION 2.0.9)
set(SDL_DIR ${EXTERNAL_DIR}/SDL2-${SDL_VERSION}/)
set(SDL_STATIC ON)
set(SDL_SHARED OFF)
add_subdirectory(${SDL_DIR})
include_directories(${SDL2_DIR}/include ${SDL2_DIR}/src)
LIST(APPEND NEKO_LIBRARIES
		SDL2-static)
		set_target_properties (SDL2-static PROPERTIES FOLDER Externals)
if(MSVC)
	LIST(APPEND NEKO_LIBRARIES
		SDL2main)
		set_target_properties (SDL2main PROPERTIES FOLDER Externals)
endif(MSVC)
#sfml network
set(SFML_DIR "${EXTERNAL_DIR}/SFML-2.5.1/")
include_directories("${SFML_DIR}/include/" "${SFML_DIR}/src/")
file(GLOB SFML_NETWORK_SRC ${SFML_DIR}/src/SFML/Network/*.cpp ${SFML_DIR}/src/SFML/Network/*.hpp)
if(MSVC)
	add_compile_definitions(SFML_STATIC=1)
    set(SFML_NETWORK_SRC
        ${SFML_NETWORK_SRC}
        ${SFML_DIR}/src/SFML/Network//Win32/SocketImpl.cpp
        ${SFML_DIR}/src/SFML/Network//Win32/SocketImpl.hpp
    )
else()
    set(SFML_NETWORK_SRC
        ${SFML_NETWORK_SRC}
        ${SFML_DIR}/src/SFML/Network//Unix/SocketImpl.cpp
        ${SFML_DIR}/src/SFML/Network//Unix/SocketImpl.hpp
    )
endif()
add_library(sfml-network STATIC ${SFML_NETWORK_SRC})
LIST(APPEND NEKO_LIBRARIES
		sfml-network)
set_target_properties (sfml-network PROPERTIES FOLDER Externals)
#opengl
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
LIST(APPEND NEKO_LIBRARIES
		${OPENGL_LIBRARIES})
	message("OpenGL Libraries: ${OPENGL_LIBRARIES}")
#glew
set(GLEW_DIR ${EXTERNAL_DIR}/glew)
add_compile_definitions(GLEW_STATIC)
include_directories(${GLEW_DIR}/include)
add_subdirectory(${GLEW_DIR})
LIST(APPEND NEKO_LIBRARIES
	glew)
set_target_properties (glew PROPERTIES FOLDER Externals)
	
set(IMGUI_ROOT ${EXTERNAL_DIR}/imgui/)
set(SDL2_IMGUI ${EXTERNAL_DIR}/imgui-sdl/)
add_subdirectory(${SDL2_IMGUI})
include_directories(${IMGUI_ROOT} ${SDL2_IMGUI})
LIST(APPEND NEKO_LIBRARIES
	imgui-sdl)
set_target_properties (imgui-sdl PROPERTIES FOLDER Externals)


#BOX2D
set(BOX2D_DIR ${EXTERNAL_DIR}/Box2D-2.3.1)
set(BOX2D_BUILD_EXAMPLES OFF CACHE INTERNAL "")
SET(BOX2D_INSTALL ON CACHE INTERNAL "")
SET(BOX2D_BUILD_STATIC ON  CACHE INTERNAL "")
add_subdirectory(${BOX2D_DIR})
include_directories(${BOX2D_DIR}/)
LIST(APPEND Neko_LIBRARIES
	Box2D)
	
set_target_properties (Box2D PROPERTIES FOLDER Externals)
#glm
set(GLM_DIR ${EXTERNAL_DIR}/glm)
include_directories(${GLM_DIR})
#Utility
set(UTILITIES_DIR ${EXTERNAL_DIR}/utilities/)
include_directories(${UTILITIES_DIR})
#taskflow
set(TASKFLOW_DIR ${EXTERNAL_DIR}/taskflow-2.2)
include_directories(${TASKFLOW_DIR})
#Remotery
set(REMOTERY_DIR ${EXTERNAL_DIR}/Remotery)
add_subdirectory(${REMOTERY_DIR})
include_directories(${REMOTERY_DIR})
LIST(APPEND Neko_LIBRARIES Remotery)
set_target_properties (Remotery PROPERTIES FOLDER Externals)
#Neko_COMMON

include_directories(include/)

add_library(Neko_COMMON STATIC ${Neko_SRC})
	target_compile_options(Neko_COMMON PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
		-Wall -Wextra>
		$<$<CXX_COMPILER_ID:MSVC>:
		/W4>)
target_link_libraries(Neko_COMMON PUBLIC ${Neko_LIBRARIES})
add_dependencies(Neko_COMMON DataTarget)
set_property(TARGET Neko_COMMON PROPERTY CXX_STANDARD 17)
target_compile_definitions(Neko_COMMON PUBLIC $<$<CONFIG:Debug>:__neko_dbg__>)
target_compile_definitions(Neko_COMMON PUBLIC $<$<CONFIG:RelWithDebInfo>:__neko_dbg__>)
target_compile_definitions(Neko_COMMON PUBLIC SOURCE_PATH="${CMAKE_SOURCE_DIR}")

set_target_properties (Neko_COMMON PROPERTIES FOLDER Neko)
#extensions
file(GLOB EXTENSIONS_FOLDERS extensions/*)
foreach(EXTENSION_FOLDER ${EXTENSIONS_FOLDERS})
	if(IS_DIRECTORY ${EXTENSION_FOLDER})
		get_filename_component(EXTENSION_DIR ${EXTENSION_FOLDER} NAME)
		file(GLOB_RECURSE  EXTENSION_FILES ${EXTENSION_FOLDER}/*.h ${EXTENSION_FOLDER}/*.cpp)
		include_directories("${EXTENSION_FOLDER}/include/")
		add_library(${EXTENSION_DIR} STATIC ${EXTENSION_FILES})
		target_compile_options(${EXTENSION_DIR} PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
				-Wall -Wextra>
				$<$<CXX_COMPILER_ID:MSVC>:
				/W4>)
		LIST(APPEND EXTENSIONS_LIBS ${EXTENSION_DIR})
		set_target_properties (${EXTENSION_DIR} PROPERTIES FOLDER Neko\\ExtensionsLibs)
		target_link_libraries(${EXTENSION_DIR} PUBLIC Neko_COMMON)
		add_dependencies(${EXTENSION_DIR} DataTarget)
	endif()
endforeach()

#Neko TEST
SET(Neko_TEST_DIR ${CMAKE_SOURCE_DIR}/test)
file(GLOB TEST_FILES ${Neko_TEST_DIR}/*.cpp)
add_executable(Neko_TEST ${TEST_FILES})

target_link_libraries(Neko_TEST gtest gtest_main Neko_COMMON ${EXTENSIONS_LIBS})
if (APPLE)
	target_link_libraries(Neko_TEST ${Boost_LIBRARIES})
else()
endif(APPLE)
add_dependencies(Neko_TEST DataTarget)

set_target_properties (Neko_TEST PROPERTIES FOLDER Neko)

#Benchmarks

SET(COURSE_DIR ${CMAKE_SOURCE_DIR}/benchmark)
file(GLOB COURSE_FILES ${COURSE_DIR}/*.cpp)

foreach(course_file ${COURSE_FILES} )
    # I used a simple string replace, to cut off .cpp.
    file(RELATIVE_PATH course_relative_path ${COURSE_DIR} ${course_file})
    string( REPLACE ".cpp" "" course_name ${course_relative_path} )
    add_executable(${course_name} ${course_file})
    # Make sure YourLib is linked to each app
    target_link_libraries(${course_name} PUBLIC benchmark benchmark_main Neko_COMMON ${EXTENSIONS_LIBS} )
    set_property(TARGET ${course_name} PROPERTY CXX_STANDARD 17)
	add_dependencies(${course_name} DataTarget)

	IF(MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Oi")#not forcing avx2 uhuh
    elseif(UNIX)
    set_target_properties(${course_name} PROPERTIES COMPILE_FLAGS "-save-temps -march=native")
    ENDIF(MSVC)
    set_target_properties (${course_name} PROPERTIES
        FOLDER Courses)
    IF(APPLE)
        set_target_properties(${course_name} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
                RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
    ENDIF()
endforeach()

include(cmake/neko_executable.cmake)
#Splash
add_neko_executable(SplashDebug ${CMAKE_SOURCE_DIR}/main/SplashDebug.cpp Neko_COMMON Network Splash engine2d)
#NekoEditor
add_neko_executable(NekoEditor2d ${CMAKE_SOURCE_DIR}/main/NekoEditor.cpp Neko_COMMON tools)
#Network Prediction Simulation
add_neko_executable(PredictionSimulation ${CMAKE_SOURCE_DIR}/main/PredictionSimulation.cpp Neko_COMMON Network)
