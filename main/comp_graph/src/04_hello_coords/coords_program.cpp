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
#include "04_hello_coords/coords_program.h"
#include "imgui.h"

#include "mathematics/matrix.h"
#include "mathematics/transform.h"
namespace neko
{

void HelloCoordsProgram::Init()
{
    textureManager_.Init();
    const auto& config = BasicEngine::GetInstance()->config;
    shader_.LoadFromFile(
            config.dataRootPath + "shaders/04_hello_coords/coords.vert",
            config.dataRootPath + "shaders/04_hello_coords/coords.frag");
    textureWallId_ = textureManager_.LoadTexture(config.dataRootPath+"sprites/wall.jpg");

    cube_.Init();

    // note that we're translating the scene in the reverse direction of where we want to move
    view = Mat4f::Identity;
    view = Transform3d::Translate(view, Vec3f(0.0f, 0.0f, -3.0f));


    glEnable(GL_DEPTH_TEST);

}



void HelloCoordsProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    timeSinceInit_ += dt;

    const auto& config = BasicEngine::GetInstance()->config;
    projection = Transform3d::Perspective(
        degree_t(45.0f), 
        static_cast<float>(config.windowSize.x) / config.windowSize.y, 
        0.1f, 
        100.0f);
    textureManager_.Update(dt);
}

void HelloCoordsProgram::Render()
{
    if(shader_.GetProgram() == 0)
        return;
    if(textureWall_ == INVALID_TEXTURE_NAME)
    {
        textureWall_ = textureManager_.GetTexture(textureWallId_).name;
        return;
    }
    std::lock_guard<std::mutex> lock(updateMutex_);
    shader_.Bind();
    glBindTexture(GL_TEXTURE_2D, textureWall_);
    shader_.SetMat4("view", view);
    shader_.SetMat4("projection", projection);

    for (const auto cubePosition : cubePositions)
    {
        Mat4f model = Mat4f::Identity; //model transform matrix
        model = Transform3d::Rotate(model, degree_t(timeSinceInit_.count()*45.0f), Vec3f(1.0f, 0.0f, 0.0f));
        model = Transform3d::Rotate(model, degree_t(timeSinceInit_.count()*45.0f), Vec3f(0.0f, 1.0f, 0.0f));

        model = Transform3d::Translate(model, cubePosition);
        shader_.SetMat4("model", model);
        cube_.Draw();
    }
}

void HelloCoordsProgram::Destroy()
{
    shader_.Destroy();
    cube_.Destroy();
    textureManager_.Destroy();

}

void HelloCoordsProgram::DrawImGui()
{
}

void HelloCoordsProgram::OnEvent(const SDL_Event& event)
{

}

}
