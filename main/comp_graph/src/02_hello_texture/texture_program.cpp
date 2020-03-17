//
// Created by efarhan on 27.01.20.
//

#include <engine/engine.h>
#include <gl/texture.h>
#include "02_hello_texture/texture_program.h"

namespace neko
{

void TextureProgram::Init()
{
    const auto& config = BasicEngine::GetInstance()->config;
    shader_.LoadFromFile(
            config.dataRootPath + "data/shaders/02_hello_texture/texture.vert",
            config.dataRootPath + "data/shaders/02_hello_texture/texture.frag");
    quad_.Init();

    const auto texturePath = config.dataRootPath + "data/sprites/wall.jpg";
    textureId_ = neko::gl::stbCreateTexture(texturePath);

    glEnable(GL_DEPTH_TEST);
}

void TextureProgram::Update(seconds dt)
{
    
}

void TextureProgram::Destroy()
{
    quad_.Destroy();
    shader_.Destroy();
    gl::DestroyTexture(textureId_);
    textureId_ = 0;
}

void TextureProgram::Render()
{
    if(shader_.GetProgram() == 0)
        return;
    shader_.Bind();
    shader_.SetInt("outTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId_);
    quad_.Draw();
}

void TextureProgram::DrawImGui()
{
	
}

void TextureProgram::OnEvent(const SDL_Event& event)
{

}
}