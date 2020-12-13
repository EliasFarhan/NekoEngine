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
            perlinValues_[i][j] = float(perlinNoise.normalizedOctaveNoise2D_0_1(x, y, currentOctave_));
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