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
            config.dataRootPath + "data/shaders/02_hello_texture/texture.vert",
            config.dataRootPath + "data/shaders/02_hello_texture/texture.frag");
    quad_.Init();

    auto texturePath = config.dataRootPath + "data/sprites/wall.jpg";
    textureId_ = neko::gl::stbCreateTexture(texturePath.c_str());
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
    shader_.SetInt("outTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId_);
    quad_.Draw();
}

void HelloTextureProgram::DrawUi()
{

}

void HelloTextureProgram::OnEvent(const SDL_Event& event)
{

}
}