
if(Neko_Assimp)
    find_package(assimp CONFIG REQUIRED)
    LIST(APPEND GLES3_WRAPPER_LIBS assimp::assimp)
endif()

find_package(freetype CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(gli CONFIG REQUIRED)
LIST(APPEND GLES3_WRAPPER_LIBS Neko_Core freetype glm::glm gli sdl_engine)

if(NOT Neko_Android)
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    message("OpenGL Libraries: ${OPENGL_LIBRARIES}")
    LIST(APPEND GLES3_WRAPPER_LIBS "${OPENGL_LIBRARIES}" GLEW::GLEW)
endif()

if(Neko_Android)
    LIST(APPEND GLES3_WRAPPER_LIBS GLEW::GLEW)
endif()

file(GLOB_RECURSE gles3_wrapper_src include/*.h src/*.cpp)
add_library(gles3_wrapper STATIC ${gles3_wrapper_src})
target_link_libraries(gles3_wrapper PUBLIC ${GLES3_WRAPPER_LIBS})
if(NOT Neko_Android)
    target_include_directories(gles3_wrapper PUBLIC ${OPENGL_INCLUDE_DIR})
endif()
target_include_directories(gles3_wrapper PUBLIC "include/")

target_precompile_headers(gles3_wrapper PUBLIC "include/gles3_wrapper_pch.h")
neko_lib_config(gles3_wrapper)
set_target_properties (gles3_wrapper PROPERTIES FOLDER Neko/Common)