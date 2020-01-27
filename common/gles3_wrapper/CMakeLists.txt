
#opengl
LIST(APPEND GLES3_WRAPPER_LIBS Neko_Core)
if(NOT Emscripten)
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})
    message("OpenGL Libraries: ${OPENGL_LIBRARIES}")
    add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD=1)
    LIST(APPEND GLES3_WRAPPER_LIBS "${OPENGL_LIBRARIES}" glad)
endif()
add_compile_definitions(IMGUI_IMPL_OPENGL_ES3=1)
if(Emscripten)

    add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_CUSTOM=1)
endif()



file(GLOB_RECURSE gles3_wrapper_src include/*.h src/*.cpp)
add_library(gles3_wrapper STATIC ${gles3_wrapper_src})
target_link_libraries(gles3_wrapper PUBLIC ${GLES3_WRAPPER_LIBS})

target_include_directories(gles3_wrapper PUBLIC "include/")
target_include_directories(gles3_wrapper PUBLIC "${EXTERNAL_DIR}/imgui-1.74")
target_include_directories(gles3_wrapper PUBLIC "${EXTERNAL_DIR}/glad/include")
neko_lib_config(gles3_wrapper)