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
		neko::Texture(std::move(tex))
    {
        if (!tex.IsLoaded())
        {
            //If you are crashing here, it means you are moving the Texture before it is fully loaded!!!
            std::abort();
        }
    }
   

    void Destroy() override;
protected:
    void CreateTexture() override;

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

TextureId gliCreateTexture(const std::string_view filename, Texture::TextureFlags flags = Texture::DEFAULT);
TextureId stbCreateTexture(const std::string_view filename, Texture::TextureFlags flags = Texture::DEFAULT);
TextureId LoadCubemap(std::vector<std::string> facesFilename);
void DestroyTexture(TextureId);



}