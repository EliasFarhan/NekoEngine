cmake_minimum_required(VERSION 3.10)
project(Neko)


set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/externals)

#data

file(GLOB_RECURSE JSON_FILES "${PROJECT_SOURCE_DIR}/data/*.json")
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
LIST(APPEND DATA_FILES ${IMG_FILES} ${SND_FILES} ${JSON_FILES})
foreach(DATA ${DATA_FILES})
	get_filename_component(FILE_NAME ${DATA} NAME)
	get_filename_component(PATH_NAME ${DATA} DIRECTORY)
	get_filename_component(EXTENSION ${DATA} EXT)
	file(RELATIVE_PATH PATH_NAME "${PROJECT_SOURCE_DIR}/shaders/" ${PATH_NAME})
	#MESSAGE("GLSL PATH: ${PATH_NAME} NAME: ${FILE_NAME}")
	set(DATA_OUTPUT "${PROJECT_BINARY_DIR}/data/${PATH_NAME}/${FILE_NAME}")

	add_custom_command(
			OUTPUT ${DATA_OUTPUT}
			COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/data/"
			COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/data/${PATH_NAME}"
			COMMAND ${CMAKE_COMMAND} -E copy ${DATA} "${PROJECT_BINARY_DIR}/data/${PATH_NAME}/${FILE_NAME}")
	list(APPEND DATA_BINARY_FILES ${DATA_OUTPUT})
endforeach(DATA)
add_custom_target(
		DataTarget
		DEPENDS ${DATA_BINARY_FILES}
)
#source files
file(GLOB_RECURSE Neko_ENGINE_SRC src/engine/*.cpp include/engine/*.h)
file(GLOB_RECURSE Neko_GRAPHICS_SRC src/graphics/*.cpp include/graphics/*.h)
file(GLOB_RECURSE Neko_UTILITY_SRC src/utilities/*.cpp include/utilities/*.h)
file(GLOB_RECURSE Neko_SOUND_SRC src/sound/*.cpp include/sound/*.h)
file(GLOB_RECURSE Neko_EXTENSIONS_SRC extensions/src/*.cpp extensions/include/*.h)

include_directories(include ${CMAKE_SOURCE_DIR}/include)
set_property(GLOBAL PROPERTY USE_FOLDERS On)
SET_SOURCE_FILES_PROPERTIES( ${Neko_SRC} PROPERTIES
                FOLDER "${CMAKE_SOURCE_DIR}/src" )

source_group("Graphics"				FILES ${Neko_GRAPHICS_SRC})
source_group("Engine"				FILES ${Neko_ENGINE_SRC})
source_group("Utility"				FILES ${Neko_UTILITY_SRC})
source_group("Sound"				FILES ${Neko_SOUND_SRC})
source_group("Data\\Json"           FILES ${JSON_FILES})
source_group("Data\\Img"            FILES ${IMG_FILES})
source_group("Data\\Snd"			FILES ${SND_FILES})

List(APPEND Neko_SRC 
	${Neko_ENGINE_SRC} 
	${Neko_GRAPHICS_SRC} 
	${Neko_UTILITY_SRC} 
	${Neko_PHYSICS_SRC}
	${Neko_SOUND_SRC}
	${JSON_FILES} 
	${IMG_FILES}
	${SND_FILES})


#SFML
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
LIST(APPEND Neko_LIBRARIES
		sfml-audio sfml-graphics sfml-window sfml-system)

#Google Test
find_package(GTest CONFIG REQUIRED)
#SFML Imgui
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
LIST(APPEND Neko_LIBRARIES ImGui-SFML::ImGui-SFML)
#glm
find_package(glm CONFIG REQUIRED)
LIST(APPEND Neko_LIBRARIES  glm::glm)
#Utility
set(UTILITIES_DIR ${EXTERNAL_DIR}/utilities/)
include_directories(${UTILITIES_DIR})
#taskflow
set(TASKFLOW_DIR ${EXTERNAL_DIR}/taskflow-2.2)
include_directories(${TASKFLOW_DIR})
#Neko_COMMON

include_directories(include/ extensions/include/)

add_library(Neko_COMMON STATIC ${Neko_SRC})
target_link_libraries(Neko_COMMON PUBLIC ${Neko_LIBRARIES})
add_dependencies(Neko_COMMON DataTarget)
set_target_properties(Neko_COMMON PROPERTIES UNITY_BUILD ON)
target_compile_definitions(Neko_COMMON PUBLIC $<$<CONFIG:Debug>:__neko_dbg__>)
target_compile_definitions(Neko_COMMON PUBLIC $<$<CONFIG:RelWithDebInfo>:__neko_dbg__>)
#Neko TEST
SET(Neko_TEST_DIR ${CMAKE_SOURCE_DIR}/test)
file(GLOB TEST_FILES ${Neko_TEST_DIR}/*.cpp )
add_executable(Neko_TEST ${TEST_FILES})
target_link_libraries(Neko_TEST GTest::gtest GTest::gtest_main Neko_COMMON City)
#extensions
file(GLOB EXTENSIONS_FOLDERS extensions/src/* extensions/include/*)
foreach(EXTENSION_FOLDER ${EXTENSIONS_FOLDERS})
	if(IS_DIRECTORY ${EXTENSION_FOLDER})
		get_filename_component(EXTENSION_DIR ${EXTENSION_FOLDER} NAME)
		file(GLOB_RECURSE  EXTENSION_FILES ${EXTENSION_FOLDER}/*.h ${EXTENSION_FOLDER}/*.cpp)
		source_group("Extensions\\${EXTENSION_DIR}"            FILES ${EXTENSION_FILES})
		if(NOT TARGET ${EXTENSION_DIR})
			add_library(${EXTENSION_DIR} STATIC ${EXTENSION_FILES})
			
			set_target_properties(${EXTENSION_DIR} PROPERTIES UNITY_BUILD ON)
			target_link_libraries(${EXTENSION_DIR} PUBLIC Neko_COMMON)
		else()
			target_sources(${EXTENSION_DIR} PRIVATE ${EXTENSION_FILES})
		endif()
	endif()
endforeach()

#City Builder
add_executable(CityBuilder ${CMAKE_SOURCE_DIR}/main/CityBuilder.cpp)
target_link_libraries(CityBuilder PUBLIC Neko_COMMON City)
set_target_properties (CityBuilder PROPERTIES FOLDER Main)
