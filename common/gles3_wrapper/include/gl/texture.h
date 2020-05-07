#pragma once

#include <string_view>
#include <graphics/texture.h>
#include "gl/gles3_include.h"

namespace neko::gl
{


enum TextureFlags : unsigned
{
    SMOOTH_TEXTURE = 1u << 0u,
    MIPMAPS_TEXTURE = 1u << 1u,
    CLAMP_WRAP = 1u << 2u,
    REPEAT_WRAP = 1u << 3u,
    MIRROR_REPEAT_WRAP = 1u << 4u,

    DEFAULT = SMOOTH_TEXTURE | MIPMAPS_TEXTURE,
};

class Texture : public neko::Texture
{
public:
    void SetTextureFlags(TextureFlags textureFlags){flags_ = textureFlags;}
    void LoadTextureToGpu() override;

    void Destroy() override;

private:
    TextureFlags flags_;
};

TextureId stbCreateTexture(const std::string_view filename, TextureFlags flags = DEFAULT);

void DestroyTexture(TextureId);



}