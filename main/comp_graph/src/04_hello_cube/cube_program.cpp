#include "imgui.h"
#include "engine/engine.h"
#include "04_hello_cube/cube_program.h"

namespace neko
{
void HelloCubeProgram::Init()
{

    const auto& config = BasicEngine::GetInstance()->config;
    shaderProgram_.LoadFromFile(
        config.dataRootPath + "data/shaders/03_hello_coords/transform.vert",
        config.dataRootPath + "data/shaders/03_hello_coords/transform.frag"
    );
    const auto texturePath = config.dataRootPath + "data/sprites/wall.jpg";
    textureWall_ = gl::stbCreateTexture(texturePath.c_str());
    cube_.Init();
}

void HelloCubeProgram::Render()
{


    shaderProgram_.Bind();

    unsigned int transformLoc = glGetUniformLocation(shaderProgram_.GetProgram(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform_[0][0]);

    glBindTexture(GL_TEXTURE_2D, textureWall_);
    cube_.Draw();

}

void HelloCubeProgram::Destroy()
{
    cube_.Destroy();
    gl::DestroyTexture(textureWall_);
}

void HelloCubeProgram::Update(seconds dt)
{
    transform_ = Mat4f::Identity;
    transform_ = Mat4f::Translate(transform_, position_);
    transform_ = Mat4f::Scale(transform_, scale_);
    transform_ = Mat4f::Rotate(transform_, eulerAngle_);

}

void HelloCubeProgram::DrawUi(seconds dt)
{
    ImGui::Begin("Transform Window");
    ImGui::InputFloat3("Position", &position_[0]);
    ImGui::InputFloat3("Scale", &scale_[0]);
    ImGui::InputFloat3("Rotation", &eulerAngle_[0]);
    ImGui::End();
}

void HelloCubeProgram::OnEvent(const SDL_Event& event)
{

}
}