//
// Created by efarhan on 12/4/20.
//
#include "tools/perlin_viewer.h"

#include "imgui.h"
#include "PerlinNoise.hpp"

#include "engine/engine.h"
#include <graphics/texture.h>

namespace neko::voxel
{

void PerlinViewer::Init()
{
    perlinValues_.resize(textureSize_);
    Job renderInit([this]()
    {
        const auto& config = BasicEngine::GetInstance()->GetConfig();
        viewerShader_.LoadFromFile(config.dataRootPath+"shaders/tools/perlin_viewer.vert",
                                   config.dataRootPath+"shaders/tools/perlin_viewer.frag");
        renderQuad_.Init();
        ReloadTexture();

    });
    BasicEngine::GetInstance()->ScheduleJob(&renderInit, JobThreadType::RENDER_THREAD);
    renderInit.Join();
}

void PerlinViewer::Update(seconds dt)
{
    RendererLocator::get().Render(this);
}

void PerlinViewer::Destroy()
{
    Job renderDestroy([this]()
                   {
                       const auto& config = BasicEngine::GetInstance()->GetConfig();
                       viewerShader_.Destroy();
                       renderQuad_.Destroy();
                       glDeleteTextures(1, &perlinTexture_);
                   });
    BasicEngine::GetInstance()->ScheduleJob(&renderDestroy, JobThreadType::RENDER_THREAD);
    renderDestroy.Join();
}

void PerlinViewer::DrawImGui()
{
    ImGui::Begin("Perlin Viewer");
    if(perlinTexture_ != neko::INVALID_TEXTURE_NAME)
    {
        bool update = false;
        update = ImGui::InputFloat("Scale", &currentScale_, 1.0f);
        update = update || ImGui::InputInt("Octave", &currentOctave_, 1);
        update = update || ImGui::Button("Reload");
        if(update)
        {
            ReloadTexture();
        }
    }
    ImGui::End();
}

void PerlinViewer::ReloadTexture()
{
    if(perlinTexture_ != neko::INVALID_TEXTURE_NAME)
    {
        glDeleteTextures(1, &perlinTexture_);
    }
    siv::PerlinNoise perlinNoise;
    for(size_t i = 0; i < textureSize_; i++)
    {
        for(size_t j = 0; j < textureSize_; j++)
        {
            const float x = float(i) / textureSize_ * currentScale_;
            const float y = float(j) / textureSize_ * currentScale_;
            perlinValues_[i][j] = float(perlinNoise.octaveNoise(x, y, currentOctave_)) / 2.0f + 0.5f;
        }
    }
    glGenTextures(1, &perlinTexture_);

    glBindTexture(GL_TEXTURE_2D, perlinTexture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, textureSize_, textureSize_, 0, GL_RED, GL_FLOAT, &perlinValues_[0][0]);
    glBindTexture(GL_TEXTURE_2D, neko::INVALID_TEXTURE_NAME);
}

void PerlinViewer::Render()
{
    viewerShader_.Bind();
    viewerShader_.SetTexture("ourTexture", perlinTexture_);
    renderQuad_.Draw();
}
}