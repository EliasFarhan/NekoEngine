
#opengl
LIST(APPEND vk_WRAPPER_LIBS Neko_Core sdl_engine)

if(NOT Neko_Android)
	find_package(Vulkan REQUIRED)
    message("Vulkan Libraries: ${Vulkan_LIBRARIES} Include: ${Vulkan_INCLUDE_DIRS}")
    LIST(APPEND vk_WRAPPER_LIBS "${Vulkan_LIBRARIES}")
endif()


if(Neko_Android)
    LIST(APPEND vk_WRAPPER_LIBS glad)
endif()



file(GLOB_RECURSE vk_wrapper_src include/*.h src/*.cpp)
add_library(vk_wrapper STATIC ${vk_wrapper_src})
target_link_libraries(vk_wrapper PUBLIC ${vk_WRAPPER_LIBS})

target_include_directories(vk_wrapper PUBLIC "include/")
target_include_directories(vk_wrapper PUBLIC "${EXTERNAL_DIR}/gli/")
target_include_directories(vk_wrapper PUBLIC "${EXTERNAL_DIR}/glm/")

target_include_directories(vk_wrapper PUBLIC "${Vulkan_INCLUDE_DIRS}")
neko_lib_config(vk_wrapper)
set_target_properties (vk_wrapper PROPERTIES FOLDER Neko/Common)