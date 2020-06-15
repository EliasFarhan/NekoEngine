//
// Created by efarhan on 27.01.20.
//

#include <engine/engine.h>
#include <gl/texture.h>
#include "02_hello_texture/texture_program.h"
#include "imgui.h"
namespace neko
{

void HelloTextureProgram::Init()
{
    const auto& config = BasicEngine::GetInstance()->config;
    shader_.LoadFromFile(
            config.dataRootPath + "shaders/02_hello_texture/texture.vert",
            config.dataRootPath + "shaders/02_hello_texture/texture.frag");
    quad_.Init();

    const auto texturePath = config.dataRootPath + "sprites/wall.jpg";
    texture_.SetPath(texturePath);
    texture_.LoadFromDisk();
    auto& textureManager = TextureManagerLocator::get();
	//textureId_ = neko::gl::stbCreateTexture(texturePath);
    ddsTexture_ = gl::gliCreateTexture(config.dataRootPath + "sprites/wall.dds");
    ktxTexture_ = gl::gliCreateTexture(config.dataRootPath + "sprites/wall.ktx");
    glEnable(GL_DEPTH_TEST);
}

void HelloTextureProgram::Update(seconds dt)
{

}

void HelloTextureProgram::Destroy()
{
    quad_.Destroy();
    shader_.Destroy();
    texture_.Destroy();
}

void HelloTextureProgram::Render()
{
    if (shader_.GetProgram() == 0)
    {
        return;
    }
	if (texture_.GetTextureId() == INVALID_TEXTURE_ID)
	{
        return;
	}
    shader_.Bind();
    shader_.SetInt("ourTexture", 0);//set the texture slot
    glActiveTexture(GL_TEXTURE0);//activate the texture slot
    switch (textureType_)
    {
    case TextureType::STB_TEXTURE:
    {
        glBindTexture(GL_TEXTURE_2D, texture_.GetTextureId());//bind texture id to texture slot
        break;
    }
    case TextureType::DDS_TEXTURE:
        glBindTexture(GL_TEXTURE_2D, ddsTexture_);//bind texture id to texture slot
    	break;
    case TextureType::KTX_TEXTURE:
        glBindTexture(GL_TEXTURE_2D, ktxTexture_);//bind texture id to texture slot
    	break;
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