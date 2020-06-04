#pragma once

#include <string_view>
#include <graphics/texture.h>
#include "gl/gles3_include.h"

namespace neko::gl
{


class Texture : public neko::Texture
{
public:
    Texture() : neko::Texture(){}

    Texture(Texture&& tex) noexcept :
		neko::Texture(std::move(tex)),
		flags_(tex.flags_)
    {
        if (!tex.IsLoaded())
        {
            //If you are crashing here, it means you are moving the Texture before it is fully loaded!!!
            std::abort();
        }
    }
    enum TextureFlags : unsigned
    {
        SMOOTH_TEXTURE = 1u << 0u,
        MIPMAPS_TEXTURE = 1u << 1u,
        CLAMP_WRAP = 1u << 2u,
        REPEAT_WRAP = 1u << 3u,
        MIRROR_REPEAT_WRAP = 1u << 4u,

        DEFAULT = SMOOTH_TEXTURE | MIPMAPS_TEXTURE,
    };
    void SetTextureFlags(TextureFlags textureFlags){flags_ = textureFlags;}
    void Destroy() override;
protected:
    void CreateTexture() override;

    TextureFlags flags_ = DEFAULT;
};

class TextureManager : public neko::TextureManager
{
public:
    TextureManager();
	TextureIndex LoadTexture(std::string_view path) override;

	void Update(seconds dt) override;
	void Destroy() override;

	TextureId GetTextureId(TextureIndex index) override;
protected:
    std::vector<std::unique_ptr<Texture>> textures_;
};

TextureId stbCreateTexture(const std::string_view filename, Texture::TextureFlags flags = Texture::DEFAULT);
TextureId LoadCubemap(std::vector<std::string> facesFilename);
void DestroyTexture(TextureId);



}