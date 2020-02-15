#pragma once

#include <string_view>
#include "gl/gles3_include.h"

namespace neko::gl
{

using TextureId = GLuint;

enum TextureFlags : unsigned
{
    NO_FLAGS = 0,
    SMOOTH_TEXTURE = 1u << 0u,
    MIPMAPS_TEXTURE = 1u << 1u,
    CLAMP_WRAP_TEXTURE = 1u << 2u
};

TextureId stbCreateTexture(const std::string_view filename, TextureFlags flags = NO_FLAGS);
void DestroyTexture(TextureId);

}