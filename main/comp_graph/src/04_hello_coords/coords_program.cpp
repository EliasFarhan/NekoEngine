#include <engine/engine.h>
#include "04_hello_coords/coords_program.h"
#include "imgui.h"

#include "mathematics/matrix.h"
#include "mathematics/transform.h"

namespace neko
{

void HelloCoordsProgram::Init()
{
    const auto& config = BasicEngine::GetInstance()->config;
    shader_.LoadFromFile(
            config.dataRootPath+"data/shaders/04_hello_coords/coords.vert",
            config.dataRootPath+"data/shaders/04_hello_coords/coords.frag");
    textureWall_ = gl::stbCreateTexture(config.dataRootPath+"data/sprites/wall.jpg");
    cube_.Init();

    // note that we're translating the scene in the reverse direction of where we want to move
    view = Mat4f::Identity;
    view = Mat4f::Translate(view, Vec3f(0.0f, 0.0f, -3.0f));


    glEnable(GL_DEPTH_TEST);

}



void HelloCoordsProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    timeSinceInit_ += dt;

    const auto& config = BasicEngine::GetInstance()->config;
    projection = Mat4f::Perspective(
        degree_t(45.0f), 
        static_cast<float>(config.windowSize.x) / config.windowSize.y, 
        0.1f, 
        100.0f);

}

void HelloCoordsProgram::Render()
{
    if(shader_.GetProgram() == 0)
        return;
    std::lock_guard<std::mutex> lock(updateMutex_);
    shader_.Bind();
    glBindTexture(GL_TEXTURE_2D, textureWall_);
    shader_.SetMat4("view", view);
    shader_.SetMat4("projection", projection);

    for (auto cubePosition : cubePositions)
    {
        Mat4f model = Mat4f::Identity; //model transform matrix
        model = Mat4f::Rotate(model, degree_t(timeSinceInit_.count()*45.0f), Vec3f(1.0f, 0.0f, 0.0f));
        model = Mat4f::Rotate(model, degree_t(timeSinceInit_.count()*45.0f), Vec3f(0.0f, 1.0f, 0.0f));

        model = Mat4f::Translate(model, cubePosition);
        shader_.SetMat4("model", model);
        cube_.Draw();
    }
}

void HelloCoordsProgram::Destroy()
{
    shader_.Destroy();
    cube_.Destroy();
    gl::DestroyTexture(textureWall_);

}

void HelloCoordsProgram::DrawImGui()
{
}

void HelloCoordsProgram::OnEvent(const SDL_Event& event)
{

}

}
