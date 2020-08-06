#pragma once
/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
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