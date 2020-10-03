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
namespace neko
{

void HelloTextureProgram::Init()
{
    textureManager_.Init();
    const auto& config = BasicEngine::GetInstance()->config;
    shader_.LoadFromFile(
            config.dataRootPath + "shaders/02_hello_texture/texture.vert",
            config.dataRootPath + "shaders/02_hello_texture/texture.frag");
    quad_.Init();

    const auto texturePath = config.dataRootPath + "sprites/wall.jpg";
    textureId_ = textureManager_.LoadTexture(texturePath);
	//textureId_ = neko::gl::stbCreateTexture(texturePath);
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
}

void HelloTextureProgram::Render()
{
    if (shader_.GetProgram() == 0)
    {
        return;
    }
	if (texture_ == INVALID_TEXTURE_NAME)
	{
        texture_ = textureManager_.GetTexture(textureId_).name;
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
    default: ;
    }
    quad_.Draw();
}

void HelloTextureProgram::DrawImGui()
{
    ImGui::Begin("Texture Program");
    const char* textureTypeNames[(int)TextureType::LENGTH] =
    {
    	"Stb Texture (JPG)",
    	"DDS Texture",
    	"Ktx Texture"
    };
    int textureType = static_cast<int>(textureType_);
	if(ImGui::Combo("Texture Type", &textureType, textureTypeNames, (int)TextureType::LENGTH ))
	{
        textureType_ = static_cast<TextureType>(textureType);
	}
    ImGui::End();
}

void HelloTextureProgram::OnEvent(const SDL_Event& event)
{

}
}