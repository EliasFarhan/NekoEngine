#include <engine/transform.h>
#include <engine/engine.h>
#include <imgui.h>
#include "03_hello_transform/transform_program.h"


#define ROTATE_OVER_TIME

namespace neko
{
void HelloTransformProgram::Init()
{

    const auto& config = BasicEngine::GetInstance()->config;
    shaderProgram_.LoadFromFile(
            config.dataRootPath + "data/shaders/03_hello_transform/transform.vert",
            config.dataRootPath + "data/shaders/03_hello_transform/transform.frag"
    );
    const auto texturePath = config.dataRootPath + "data/sprites/wall.jpg";
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
    transform_ = Mat4f::Scale(transform_, scale_);
    switch(shape_)
    {
        case ShapeType::PLANE:
            transform_ = Mat4f::Rotate(transform_, degree_t(angle_), Vec3f(0.0f, 0.0f, 1.0f));
            break;
        case ShapeType::CUBE:
            transform_ = Mat4f::Rotate(transform_, eulerAngle_);
            break;
        default:
            break;
    }

    transform_ = Mat4f::Translate(transform_, position_);
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
