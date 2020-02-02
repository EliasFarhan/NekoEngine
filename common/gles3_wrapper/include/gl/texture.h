#pragma once

namespace neko::gl
{

enum TextureFlags : unsigned
{
    NO_FLAGS = 0,
    SMOOTH_TEXTURE = 1 << 0,
    MIPMAPS_TEXTURE = 1 << 1,
    CLAMP_WRAP_TEXTURE = 1 << 2
};

unsigned stbCreateTexture(const char* filename, TextureFlags flags = NO_FLAGS);
}