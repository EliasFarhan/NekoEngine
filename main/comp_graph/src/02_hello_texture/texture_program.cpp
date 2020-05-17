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
    textureId_ = neko::gl::stbCreateTexture(texturePath);

    glEnable(GL_DEPTH_TEST);
}

void HelloTextureProgram::Update(seconds dt)
{

}

void HelloTextureProgram::Destroy()
{
    quad_.Destroy();
    shader_.Destroy();
    gl::DestroyTexture(textureId_);
    textureId_ = 0;
}

void HelloTextureProgram::Render()
{
    if(shader_.GetProgram() == 0)
        return;
    shader_.Bind();
    shader_.SetInt("ourTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId_);
    quad_.Draw();
}

void HelloTextureProgram::DrawImGui()
{

}

void HelloTextureProgram::OnEvent(const SDL_Event& event)
{

}
}