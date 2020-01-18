
#opengl
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
LIST(APPEND engine3d_lib
        ${OPENGL_LIBRARIES})
message("OpenGL Libraries: ${OPENGL_LIBRARIES}")

add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD=1)



file(GLOB_RECURSE gles3_wrapper_src include/*.h src/*.cpp)
add_library(gles3_wrapper STATIC ${gles3_wrapper_src})
target_link_libraries(gles3_wrapper PUBLIC ${OPENGL_LIBRARIES} glad Neko_Core)

target_include_directories(gles3_wrapper PUBLIC "include/")
target_include_directories(gles3_wrapper PUBLIC "${EXTERNAL_DIR}/glad/include")