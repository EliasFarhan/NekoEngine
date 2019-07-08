cmake_minimum_required(VERSION 3.10)
project(Neko)


set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND NOT APPLE)
	MESSAGE("Linux Compiler ${CMAKE_CXX_COMPILER_ID}")
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		MESSAGE("LLVM_ROOT ${LLVM_ROOT}")
		# using Clang

		if(LLVM_ROOT)
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  --stdlib=libc++ -L${LLVM_ROOT}/lib -Wl,-rpath,${LLVM_ROOT}/lib  -lc++ -lc++abi ")#-fsanitize=thread
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -nostdinc++")

			include_directories(${LLVM_ROOT}/include/c++/v1/)
			if("${CMAKE_CXX_COMPILER_VERSION}" STREQUAL "9.0.0")
			else()
				LIST(APPEND Neko_LIBRARIES
						c++fs)
			endif()
			SET (CMAKE_AR      "${LLVM_ROOT}/bin/llvm-ar")
			SET (CMAKE_LINKER  "${LLVM_ROOT}/bin/llvm-ld")
			SET (CMAKE_NM      "${LLVM_ROOT}/bin/llvm-nm")
			SET (CMAKE_OBJDUMP "${LLVM_ROOT}/bin/llvm-objdump")
			SET (CMAKE_RANLIB  "${LLVM_ROOT}/bin/llvm-ranlib")
		else()
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")# -fsanitize=address")
		endif()

	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # using GCC
		LIST(APPEND Neko_LIBRARIES
				stdc++fs)
	endif()
endif(UNIX AND NOT APPLE)

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
file(GLOB_RECURSE Neko_PHYSICS_SRC src/physics/*.cpp include/physics/*.h)
file(GLOB_RECURSE Neko_SOUND_SRC src/sound/*.cpp include/sound/*.h)
file(GLOB_RECURSE Neko_EXTENSIONS_SRC extensions/src/*.cpp extensions/include/*.h)

include_directories(include ${CMAKE_SOURCE_DIR}/include)
set_property(GLOBAL PROPERTY USE_FOLDERS On)
SET_SOURCE_FILES_PROPERTIES( ${Neko_SRC} PROPERTIES
                FOLDER "${CMAKE_SOURCE_DIR}/src" )

source_group("Graphics"				FILES ${Neko_GRAPHICS_SRC})
source_group("Engine"				FILES ${Neko_ENGINE_SRC})
source_group("Utility"				FILES ${Neko_UTILITY_SRC})
source_group("Physics"				FILES ${Neko_PHYSICS_SRC})
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

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:RelWithDebInfo>:/MTd>
		$<$<CONFIG:Release>:/MT> #--|
    )
endif()


#SFML
set(SFML_DIR ${EXTERNAL_DIR}/SFML-2.5.1)
set(SFML_BUILD_EXAMPLES FALSE)
set(BUILD_SHARED_LIBS FALSE)
set(SFML_STATIC_LIBRARIES TRUE)
add_subdirectory(${SFML_DIR})
include_directories(${SFML_DIR}/include ${SFML_DIR}/src)

LIST(APPEND Neko_LIBRARIES
		sfml-audio sfml-graphics sfml-window sfml-system sfml-network)

#Google Test
enable_testing()
set(GOOGLE_TEST_DIR ${EXTERNAL_DIR}/googletest-1.8.1)
set(BUILD_GMOCK OFF CACHE INTERNAL "")
set(INSTALL_GTEST OFF CACHE INTERNAL "")
add_subdirectory(${GOOGLE_TEST_DIR})
include_directories(${GOOGLE_TEST_DIR}/include)

set_target_properties (gtest gtest_main PROPERTIES
		FOLDER GTest)
#SFML Imgui
set(SFML_IMGUI ${EXTERNAL_DIR}/imgui-sfml-2.0.1/)
set(IMGUI_ROOT ${EXTERNAL_DIR}/imgui-1.70/ CACHE INTERNAL "")
set(IMGUI_DIR ${EXTERNAL_DIR}/imgui-1.70/ CACHE INTERNAL "")
set(IMGUI_SFML_FIND_SFML OFF CACHE INTERNAL "")
set(SFML_ROOT ${SFML_DIR})
add_subdirectory(${SFML_IMGUI})
include_directories(${IMGUI_ROOT} ${SFML_IMGUI})
LIST(APPEND Neko_LIBRARIES
	ImGui-SFML)

#BOX2D
set(BOX2D_DIR ${EXTERNAL_DIR}/Box2D-2.3.1)
set(BOX2D_BUILD_EXAMPLES OFF CACHE INTERNAL "")
SET(BOX2D_INSTALL ON CACHE INTERNAL "")
SET(BOX2D_BUILD_STATIC ON  CACHE INTERNAL "")

add_subdirectory(${BOX2D_DIR})
include_directories(${BOX2D_DIR}/)
LIST(APPEND Neko_LIBRARIES
	Box2D)

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

#Neko_COMMON

include_directories(include/ extensions/include/)

add_library(Neko_COMMON STATIC ${Neko_SRC})
target_link_libraries(Neko_COMMON PUBLIC ${Neko_LIBRARIES})
add_dependencies(Neko_COMMON DataTarget)
set_property(TARGET Neko_COMMON PROPERTY CXX_STANDARD 17)
target_compile_definitions(Neko_COMMON PUBLIC $<$<CONFIG:Debug>:__neko_dbg__>)
target_compile_definitions(Neko_COMMON PUBLIC $<$<CONFIG:RelWithDebInfo>:__neko_dbg__>)
#Neko TEST
SET(Neko_TEST_DIR ${CMAKE_SOURCE_DIR}/test)
file(GLOB TEST_FILES ${Neko_TEST_DIR}/*.cpp )
add_executable(Neko_TEST ${TEST_FILES})
if (APPLE)
	target_link_libraries(Neko_TEST gtest gtest_main ${Boost_LIBRARIES} Neko_COMMON City)
else()
	target_link_libraries(Neko_TEST gtest gtest_main Neko_COMMON City)
endif(APPLE)
#extensions
file(GLOB EXTENSIONS_FOLDERS extensions/src/* extensions/include/*)
foreach(EXTENSION_FOLDER ${EXTENSIONS_FOLDERS})
	if(IS_DIRECTORY ${EXTENSION_FOLDER})
		get_filename_component(EXTENSION_DIR ${EXTENSION_FOLDER} NAME)
		file(GLOB_RECURSE  EXTENSION_FILES ${EXTENSION_FOLDER}/*.h ${EXTENSION_FOLDER}/*.cpp)
		source_group("Extensions\\${EXTENSION_DIR}"            FILES ${EXTENSION_FILES})
		if(NOT TARGET ${EXTENSION_DIR})
			add_library(${EXTENSION_DIR} STATIC ${EXTENSION_FILES})
			target_link_libraries(${EXTENSION_DIR} PUBLIC Neko_COMMON)
		else()
			target_sources(${EXTENSION_DIR} PRIVATE ${EXTENSION_FILES})
		endif()
	endif()
endforeach()

#City Builder
add_executable(CityBuilder ${CMAKE_SOURCE_DIR}/main/CityBuilder.cpp)
target_link_libraries(CityBuilder PUBLIC Neko_COMMON City)
set_property(TARGET CityBuilder PROPERTY CXX_STANDARD 17)
set_target_properties (CityBuilder PROPERTIES FOLDER Main)


if(MSVC)
if(NOT CMAKE_CL_64)
file(COPY ${SFML_DIR}/extlibs/bin/x86/openal32.dll DESTINATION ${CMAKE_BINARY_DIR}/)
else()
file(COPY ${SFML_DIR}/extlibs/bin/x64/openal32.dll DESTINATION ${CMAKE_BINARY_DIR}/)
endif()
endif()
