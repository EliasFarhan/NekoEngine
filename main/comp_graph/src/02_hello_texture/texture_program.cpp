//
// Created by efarhan on 27.01.20.
//

#include <engine/engine.h>
#include <gl/texture.h>
#include "02_hello_texture/texture_program.h"

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
    glBindTexture(GL_TEXTURE_2D, texture_.GetTextureId());//bind texture id to texture slot
    quad_.Draw();
}

void HelloTextureProgram::DrawImGui()
{

}

void HelloTextureProgram::OnEvent(const SDL_Event& event)
{

}
}