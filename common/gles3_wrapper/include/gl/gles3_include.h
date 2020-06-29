#pragma once
#include "engine/log.h"

#ifdef NEKO_GLES3
#ifdef EMSCRIPTEN
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#define GL_GLEXT_PROTOTYPES 1
#endif

void CheckFramebuffer(const char* file, int line);
void CheckGlError(const char* file, int line);
#define glCheckError() CheckGlError(__FILE__, __LINE__) 
#define glCheckFramebuffer() CheckFramebuffer(__FILE__, __LINE__)
#endif