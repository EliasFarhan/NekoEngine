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

#include <engine/transform.h>
#include <engine/engine.h>
#include <imgui.h>
#include "03_hello_transform/transform_program.h"
#include "mathematics/transform.h"

#define ROTATE_OVER_TIME

namespace neko
{
void HelloTransformProgram::Init()
{

    const auto& config = BasicEngine::GetInstance()->config;
    shaderProgram_.LoadFromFile(
            config.dataRootPath + "shaders/03_hello_transform/transform.vert",
            config.dataRootPath + "shaders/03_hello_transform/transform.frag");
    const auto texturePath = config.dataRootPath + "sprites/wall.jpg";
    textureWall_ = gl::stbCreateTexture(texturePath.c_str());
    quad_.Init();
    cube_.Init();
}

void HelloTransformProgram::Render()
{
    if(shaderProgram_.GetProgram() == 0)
        return;
    std::lock_guard<std::mutex> lock(updateMutex_);

    shaderProgram_.Bind();

    shaderProgram_.SetMat4("transform", transform_);

    glBindTexture(GL_TEXTURE_2D, textureWall_);
    switch (shape_)
    {

        case ShapeType::PLANE:
            quad_.Draw();
            break;
        case ShapeType::CUBE:
            cube_.Draw();
            break;
        default:
            break;
    }

    glEnable(GL_DEPTH_TEST);

}

void HelloTransformProgram::Destroy()
{
    cube_.Destroy();
    quad_.Destroy();
    gl::DestroyTexture(textureWall_);
    shaderProgram_.Destroy();
}

void HelloTransformProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    transform_ = Mat4f::Identity;
    transform_ = Transform3d::Scale(transform_, scale_);
    switch(shape_)
    {
        case ShapeType::PLANE:
            transform_ = Transform3d::Rotate(transform_, degree_t(angle_), Vec3f(0.0f, 0.0f, 1.0f));
            break;
        case ShapeType::CUBE:
            transform_ = Transform3d::Rotate(transform_, eulerAngle_);
            break;
        default:
            break;
    }

    transform_ = Transform3d::Translate(transform_, position_);
}

void HelloTransformProgram::DrawImGui()
{

    ImGui::SetNextWindowPos(ImVec2(0, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Transform Window");
    const char* items[static_cast<size_t>(ShapeType::LENGTH)] = {
            "Plane",
            "Cube",
    };
    int currentIndex = static_cast<int>(shape_);
    if (ImGui::Combo("Render Types", &currentIndex, items, (size_t) ShapeType::LENGTH))
    {
        shape_ = static_cast<ShapeType>(currentIndex);
    }
    ImGui::InputFloat3("Position", &position_[0]);
    ImGui::InputFloat3("Scale", &scale_[0]);
    switch (shape_)
    {
        case ShapeType::PLANE:
            ImGui::InputFloat("Rotation", (float*) &angle_);
            break;
        case ShapeType::CUBE:
            ImGui::InputFloat3("Euler Angles", (float*) &eulerAngle_[0]);
            break;
        default:
            break;
    }
    ImGui::End();
}

void HelloTransformProgram::OnEvent(const SDL_Event& event)
{

}

}
