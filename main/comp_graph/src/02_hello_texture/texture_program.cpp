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
#include <engine/engine.h>
#include <gl/texture.h>
#include "02_hello_texture/texture_program.h"
#include "imgui.h"
#include <gli/gli.hpp>

namespace neko
{

void HelloTextureProgram::Init()
{
    textureManager_.Init();
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    const auto& filesystem = BasicEngine::GetInstance()->GetFilesystem();
    shader_.LoadFromFile(
            config.dataRootPath + "shaders/02_hello_texture/texture.vert",
            config.dataRootPath + "shaders/02_hello_texture/texture.frag");
    quad_.Init();

    const auto texturePath = config.dataRootPath + "sprites/wall.jpg";
    textureId_ = textureManager_.LoadTexture(texturePath, gl::Texture::DEFAULT);
    texture_ = gl::StbCreateTexture(texturePath, filesystem, gl::Texture::DEFAULT);
	//textureId_ = neko::gl::stbCreateTexture(texturePath);

    gliTextureName_ = gl::CreateTextureFromKTX(texturePath + ".ktx", filesystem);
    glEnable(GL_DEPTH_TEST);
}

void HelloTextureProgram::Update(seconds dt)
{
    textureManager_.Update(dt);
}

void HelloTextureProgram::Destroy()
{
    quad_.Destroy();
    shader_.Destroy();
    textureManager_.Destroy();
    if(texture_ != gl::INVALID_TEXTURE_NAME)
        gl::DestroyTexture(texture_);
    gl::DestroyTexture(gliTextureName_);
}

void HelloTextureProgram::Render()
{
    if (shader_.GetProgram() == 0)
    {
        return;
    }
	if (textureKtx_ == gl::INVALID_TEXTURE_NAME)
	{
        textureKtx_ = textureManager_.GetTextureName(textureId_);
        return;
	}
    shader_.Bind();
    shader_.SetInt("ourTexture", 0);//set the texture slot
    glActiveTexture(GL_TEXTURE0);//activate the texture slot
    switch (textureType_)
    {
    case TextureType::STB_TEXTURE:
    {
        glBindTexture(GL_TEXTURE_2D, texture_);//bind texture id to texture slot
        break;
    }
    case TextureType::KTX_TEXTURE:
    {
        glBindTexture(GL_TEXTURE_2D, textureKtx_);//bind texture id to texture slot
        break;
    }
    case TextureType::GLI_TEXTURE:
    {
        glBindTexture(GL_TEXTURE_2D, gliTextureName_);
        break;
    }
    default: break;
    }
    quad_.Draw();
}

void HelloTextureProgram::DrawImGui()
{
    ImGui::Begin("Texture Program");
    const char* textureTypeNames[(int)TextureType::LENGTH] =
    {
    	"Stb Texture (JPG)",
    	"Ktx Texture",
        "Ktx Texture With GLI"
    };
    int textureType = static_cast<int>(textureType_);
	if(ImGui::Combo("Texture Type", &textureType, textureTypeNames, static_cast<int>(TextureType::LENGTH) ))
	{
        textureType_ = static_cast<TextureType>(textureType);
	}
    ImGui::End();
}

void HelloTextureProgram::OnEvent(const SDL_Event& event)
{

}

gl::TextureName HelloTextureProgram::LoadTextureWithGli(std::string_view path)
{
    gli::texture texture = gli::load(path.data());
    if (texture.empty())
    {
        logError("Could not load texture with GLI");
        return gl::INVALID_TEXTURE_NAME;
    }
    gli::gl glProfile(gli::gl::PROFILE_ES30);
    const gli::gl::format format = glProfile.translate(texture.format(), texture.swizzles());
    GLenum target = glProfile.translate(texture.target());
    logDebug(fmt::format("texture format: {}, texture target {}, is compressed {}", 
        texture.format(), 
        texture.target(),
        is_compressed(texture.format())));
    neko_assert(gli::is_compressed(texture.format()) && texture.target() == gli::TARGET_2D, "Is compressed and Target 2d");

    GLuint textureName = 0;
    glGenTextures(1, &textureName);
    glBindTexture(target, textureName);
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(texture.levels() - 1));
    
    glm::tvec3<GLsizei> extent{ texture.extent() };
    glTexStorage2D(target, static_cast<GLint>(texture.levels()), format.Internal, extent.x, extent.y);
    for (std::size_t level = 0; level < texture.levels(); ++level)
    {
        glm::tvec3<GLsizei> levelExtent(texture.extent(level));
        glCompressedTexSubImage2D(
            target, static_cast<GLint>(level), 0, 0, levelExtent.x, levelExtent.y,
            format.Internal, static_cast<GLsizei>(texture.size(level)), texture.data(0, 0, level));
    }

    return textureName;
}
}
