#pragma once
#ifdef NEKO_GLES3

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#define GL_GLEXT_PROTOTYPES 1
#endif

#endif