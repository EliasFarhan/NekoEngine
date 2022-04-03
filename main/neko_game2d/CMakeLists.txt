if(NOT Neko_GLES3 and NOT Neko_Box2D and NOT Neko_SDL2)
	return
endif()

file(GLOB_RECURSE neko_game2d_src include/*.h src/*.cpp)

add_executable(neko_game2d ${neko_game2d_src})

target_include_directories(neko_game2d PUBLIC "include/")

target_compile_options(neko_game2d PRIVATE ${NEKO_COMPILE_FLAGS})
target_link_options(neko_game2d PRIVATE ${NEKO_LINK_FLAGS})
set_target_properties (neko_game2d PROPERTIES FOLDER Neko/Main)
set_target_properties(neko_game2d PROPERTIES UNITY_BUILD ON)
target_link_libraries(neko_game2d PUBLIC Neko_Core sdl_engine gles3_wrapper b2_wrapper)