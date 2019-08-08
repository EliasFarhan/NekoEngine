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
			LIST(APPEND Neko_LIBRARIES
					stdc++fs)
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

file(GLOB_RECURSE TEXT_FILES 
"${PROJECT_SOURCE_DIR}/data/*.json" 
"${PROJECT_SOURCE_DIR}/data/*.txt"
"${PROJECT_SOURCE_DIR}/data/*.scene"
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
LIST(APPEND DATA_FILES ${IMG_FILES} ${SND_FILES} ${TEXT_FILES})
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
source_group("Data\\Text"           FILES ${TEXT_FILES})
source_group("Data\\Img"            FILES ${IMG_FILES})
source_group("Data\\Snd"			FILES ${SND_FILES})

List(APPEND Neko_SRC 
	${Neko_ENGINE_SRC} 
	${Neko_GRAPHICS_SRC} 
	${Neko_UTILITY_SRC} 
	${Neko_PHYSICS_SRC}
	${Neko_SOUND_SRC}
	${TEXT_FILES} 
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
		FOLDER Externals\\GTest)
#Google benchmark
set(GOOGLE_BENCH_DIR ${EXTERNAL_DIR}/benchmark-1.5.0)
add_subdirectory(${GOOGLE_BENCH_DIR})
include_directories(${GOOGLE_BENCH_DIR}/include)
set_target_properties (benchmark benchmark_main PROPERTIES
		FOLDER Externals\\GTest)
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
	
set_target_properties (ImGui-SFML PROPERTIES FOLDER Externals)
#Spine sfml
set(SPINE_SFML_DIR ${EXTERNAL_DIR}/spine-3.6/)
add_subdirectory(${SPINE_SFML_DIR})
include_directories(${SPINE_SFML_DIR}/spine-sfml/src ${SPINE_SFML_DIR}/spine-c/src)
LIST(APPEND Neko_LIBRARIES spine-c spine-sfml)

set_target_properties (spine-c spine-sfml PROPERTIES FOLDER Externals)
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
		source_group("Extensions\\${EXTENSION_DIR}"            FILES ${EXTENSION_FILES})
	
		include_directories("${EXTENSION_FOLDER}/include/")
		add_library(${EXTENSION_DIR} STATIC ${EXTENSION_FILES})
		LIST(APPEND EXTENSIONS_LIBS ${EXTENSION_DIR})
		set_target_properties (${EXTENSION_DIR} PROPERTIES FOLDER Neko\\ExtensionsLibs)
		target_link_libraries(${EXTENSION_DIR} PUBLIC Neko_COMMON)
		add_dependencies(${EXTENSION_DIR} DataTarget)
	endif()
endforeach()
#Neko TEST
SET(Neko_TEST_DIR ${CMAKE_SOURCE_DIR}/test)
file(GLOB TEST_FILES ${Neko_TEST_DIR}/*.cpp )
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
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 /Oi")
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
add_neko_executable(SplashDebug ${CMAKE_SOURCE_DIR}/main/SplashDebug.cpp Neko_COMMON Network Splash)
#NekoEditor
add_neko_executable(NekoEditor2d ${CMAKE_SOURCE_DIR}/main/NekoEditor.cpp Neko_COMMON tools)
if(MSVC)
if(NOT CMAKE_CL_64)
file(COPY ${SFML_DIR}/extlibs/bin/x86/openal32.dll DESTINATION ${CMAKE_BINARY_DIR}/)
else()
file(COPY ${SFML_DIR}/extlibs/bin/x64/openal32.dll DESTINATION ${CMAKE_BINARY_DIR}/)
endif()
endif()
