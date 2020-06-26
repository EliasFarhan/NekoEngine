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
            config.dataRootPath + "shaders/04_hello_coords/coords.vert",
            config.dataRootPath + "shaders/04_hello_coords/coords.frag");
    textureWall_.SetPath(config.dataRootPath+"sprites/wall.jpg");
    textureWall_.LoadFromDisk();
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

}

void HelloCoordsProgram::Render()
{
    if(shader_.GetProgram() == 0)
        return;
    if(!textureWall_.IsLoaded())
    {
        return;
    }
    std::lock_guard<std::mutex> lock(updateMutex_);
    shader_.Bind();
    glBindTexture(GL_TEXTURE_2D, textureWall_.GetTextureId());
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
    textureWall_.Destroy();

}

void HelloCoordsProgram::DrawImGui()
{
}

void HelloCoordsProgram::OnEvent(const SDL_Event& event)
{

}

}
