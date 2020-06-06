#include "18_hello_normal/normal_program.h"

namespace neko
{

void HelloNormalProgram::Init()
{
    const auto& config = BasicEngine::GetInstance()->config;
    diffuseTex_.SetPath(config.dataRootPath+"sprites/brickwall/brickwall.jpg");
    diffuseTex_.LoadFromDisk();
    normalTex_.SetPath(config.dataRootPath+"sprites/brickwall/brickwall_normal.jpg");
    normalTex_.LoadFromDisk();

    normalShader_.LoadFromFile(
            config.dataRootPath+"shaders/18_hello_normal/normal.vert",
            config.dataRootPath+"shaders/18_hello_normal/normal.frag");
    plane_.Init();
    model_.LoadModel(config.dataRootPath + "model/nanosuit2/nanosuit.obj");

    camera_.position = Vec3f(0,3,3);
    camera_.LookAt(Vec3f::zero);
}

void HelloNormalProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    const auto& config = BasicEngine::GetInstance()->config;

    camera_.SetAspect(config.windowSize.x, config.windowSize.y);
    camera_.Update(dt);
}

void HelloNormalProgram::Destroy()
{
    model_.Destroy();
}

void HelloNormalProgram::DrawImGui()
{

}

void HelloNormalProgram::Render()
{
    if(!model_.IsLoaded())
        return;
    if(!normalTex_.IsLoaded())
        return;
    if(!diffuseTex_.IsLoaded())
        return;
    std::lock_guard<std::mutex> lock(updateMutex_);

    normalShader_.Bind();
    normalShader_.SetMat4("view", camera_.GenerateViewMatrix());
    normalShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
    normalShader_.SetVec3("viewPos", camera_.position);
    normalShader_.SetVec3("lightPos", Vec3f::one);
    normalShader_.SetMat4("model", Mat4f::Identity);
    normalShader_.SetInt("texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTex_.GetTextureId());
    normalShader_.SetInt("texture_normal1", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalTex_.GetTextureId());
    plane_.Draw();
}

void HelloNormalProgram::OnEvent(const SDL_Event& event)
{
    camera_.OnEvent(event);
}

}