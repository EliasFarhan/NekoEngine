
#opengl
set(OpenGL_GL_PREFERENCE GLVND)
add_compile_definitions(USE_OPENGL=1)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
LIST(APPEND engine3d_lib
        ${OPENGL_LIBRARIES})
message("OpenGL Libraries: ${OPENGL_LIBRARIES}")