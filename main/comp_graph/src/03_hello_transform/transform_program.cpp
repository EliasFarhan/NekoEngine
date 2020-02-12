
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
            config.dataRootPath + "data/shaders/03_hello_coords/transform.vert",
            config.dataRootPath + "data/shaders/03_hello_coords/transform.frag"
    );
    const auto texturePath = config.dataRootPath + "data/sprites/wall.jpg";
    textureWall_ = gl::stbCreateTexture(texturePath.c_str());
    quad_.Init();
}

void HelloTransformProgram::Render()
{


    shaderProgram_.Bind();

    unsigned int transformLoc = glGetUniformLocation(shaderProgram_.GetProgram(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform_[0][0]);

    glBindTexture(GL_TEXTURE_2D, textureWall_);
    quad_.Draw();

}

void HelloTransformProgram::Destroy()
{
    quad_.Destroy();
    gl::DestroyTexture(textureWall_);
}

void HelloTransformProgram::Update(seconds dt)
{
    transform_ = Mat4f::Identity;
    transform_ = Mat4f::Translate(transform_, position_);
    transform_ = Mat4f::Scale(transform_, scale_);
    transform_ = Mat4f::Rotate(transform_, angle_, Vec3f(0.0f, 0.0f, 1.0f));
    // transform_ = Mat4f::Rotate(transform_, Vec3f(angle_, angle_, angle_));
}

void HelloTransformProgram::DrawUi(seconds dt)
{
    ImGui::Begin("Transform Window");
    ImGui::InputFloat3("Position", &position_[0]);
    ImGui::InputFloat3("Scale", &scale_[0]);
    ImGui::InputFloat("Rotation", &angle_);
    ImGui::End();
}

void HelloTransformProgram::OnEvent(const SDL_Event& event)
{

}

}