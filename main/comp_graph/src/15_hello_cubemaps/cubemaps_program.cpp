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

#include "15_hello_cubemaps/cubemaps_program.h"
#include "imgui.h"

namespace neko
{
void HelloCubemapsProgram::Init()
{
    textureManager_.Init();
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    skyboxCube_.Init();
    skyboxTexture_ = gl::LoadCubemap({
                                             config.dataRootPath + "sprites/skybox/right.jpg",
                                             config.dataRootPath + "sprites/skybox/left.jpg",
                                             config.dataRootPath + "sprites/skybox/top.jpg",
                                             config.dataRootPath + "sprites/skybox/bottom.jpg",
                                             config.dataRootPath + "sprites/skybox/front.jpg",
                                             config.dataRootPath + "sprites/skybox/back.jpg"
        }, BasicEngine::GetInstance()->GetFilesystem());
    skyboxTextureId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/skybox/skybox.skybox", Texture::DEFAULT);
    skyboxShader_.LoadFromFile(
        config.dataRootPath + "shaders/15_hello_cubemaps/skybox.vert",
        config.dataRootPath + "shaders/15_hello_cubemaps/skybox.frag");
    modelId_ = modelManager_.LoadModel(config.dataRootPath + "model/nanosuit2/nanosuit.obj");
    modelShader_.LoadFromFile(config.dataRootPath + "shaders/15_hello_cubemaps/model.vert",
        config.dataRootPath + "shaders/15_hello_cubemaps/model.frag");
    modelReflectionShader_.LoadFromFile(config.dataRootPath + "shaders/15_hello_cubemaps/model.vert",
        config.dataRootPath + "shaders/15_hello_cubemaps/model_reflection.frag");
    modelRefractionShader_.LoadFromFile(config.dataRootPath + "shaders/15_hello_cubemaps/model.vert",
        config.dataRootPath + "shaders/15_hello_cubemaps/model_refraction.frag");
    camera_.position = Vec3f(0, 3, 3);
    camera_.WorldLookAt(Vec3f::zero);
    cube_.Init();
    cubeTextureId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/container.jpg.ktx", Texture::DEFAULT);

}

void HelloCubemapsProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    camera_.SetAspect(config.windowSize.x, config.windowSize.y);
    camera_.Update(dt);	textureManager_.Update(dt);
}

void HelloCubemapsProgram::Destroy()
{
    glDeleteTextures(1, &skyboxTexture_);
    skyboxCube_.Destroy();
    skyboxShader_.Destroy();

    modelShader_.Destroy();
    modelReflectionShader_.Destroy();
    modelRefractionShader_.Destroy();
    cube_.Destroy();
    textureManager_.Destroy();
    modelManager_.Destroy();
}

void HelloCubemapsProgram::DrawImGui()
{
    ImGui::Begin("Hello Cubemaps");
    const char* renderModeNames_[static_cast<int>(ModelRenderMode::LENGTH)] =
    {
        "None",
        "Reflection",
        "Refraction"
    };
    int currentItem = static_cast<int>(currentRenderMode_);
    if (ImGui::Combo("Render Mode", &currentItem, renderModeNames_, static_cast<int>(ModelRenderMode::LENGTH)))
    {
        currentRenderMode_ = static_cast<ModelRenderMode>(currentItem);
    }
    switch (currentRenderMode_)
    {
    case ModelRenderMode::REFLECTION:
        ImGui::SliderFloat("Reflection Value", &reflectionValue_, 0.0f, 1.0f);
        break;
    case ModelRenderMode::REFRACTION:
        ImGui::SliderFloat("Refraction Value", &refractionValue_, 0.0f, 1.0f);
        ImGui::SliderFloat("Refractive Index", &refractiveIndex_, 1.0f, 3.0f);
        break;
    default:
        break;
    }
    ImGui::End();
}

void HelloCubemapsProgram::Render()
{
    if (!modelManager_.IsLoaded(modelId_))
    {
        return;
    }
    if (cubeTexture_ == INVALID_TEXTURE_NAME)
    {
        cubeTexture_ = textureManager_.GetTextureName(cubeTextureId_);
        if(cubeTexture_ == INVALID_TEXTURE_NAME)
            return;
    }
    if(skyboxKtxTexture_ == INVALID_TEXTURE_NAME) 
    {
        skyboxKtxTexture_ = textureManager_.GetTextureName(skyboxTextureId_);
        if(skyboxKtxTexture_ == INVALID_TEXTURE_NAME)
            return;
    }
    std::lock_guard<std::mutex> lock(updateMutex_);

    const auto view = camera_.GenerateViewMatrix();
    const auto projection = camera_.GenerateProjectionMatrix();
    auto* mod = modelManager_.GetModel(modelId_);
    //Draw model
    switch (currentRenderMode_)
    {
    case ModelRenderMode::NONE:
    {
        modelShader_.Bind();
        modelShader_.SetMat4("view", view);
        modelShader_.SetMat4("projection", projection);
        auto model = Mat4f::Identity;
        model = Transform3d::Scale(model, Vec3f(0.1f, 0.1f, 0.1f));
        modelShader_.SetMat4("model", model);
        modelShader_.SetMat4("transposeInverseModel", model.Inverse().Transpose());
       
        mod->Draw(modelShader_);
        model = Mat4f::Identity;
        model = Transform3d::Translate(model, Vec3f::left * 2.0f);
        modelShader_.SetMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture_);
        cube_.Draw();
        break;
    }
    case ModelRenderMode::REFLECTION:
    {
        modelReflectionShader_.Bind();
        modelReflectionShader_.SetMat4("view", view);
        modelReflectionShader_.SetMat4("projection", projection);
        auto model = Mat4f::Identity;
        model = Transform3d::Scale(model, Vec3f(0.1f, 0.1f, 0.1f));
        modelReflectionShader_.SetMat4("model", model);
        modelReflectionShader_.SetMat4("transposeInverseModel", model.Inverse().Transpose());
        modelReflectionShader_.SetInt("skybox", 2);
        modelReflectionShader_.SetVec3("cameraPos", camera_.position);
        modelReflectionShader_.SetFloat("reflectionValue", reflectionValue_);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture_);
        mod->Draw(modelReflectionShader_);
        model = Mat4f::Identity;
        model = Transform3d::Translate(model, Vec3f::left * 2.0f);
        modelReflectionShader_.SetMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture_);
        cube_.Draw();
        break;
    }
    case ModelRenderMode::REFRACTION:
    {
        modelRefractionShader_.Bind();
        modelRefractionShader_.SetMat4("view", view);
        modelRefractionShader_.SetMat4("projection", projection);
        auto model = Mat4f::Identity;
        model = Transform3d::Scale(model, Vec3f(0.1f, 0.1f, 0.1f));
        modelRefractionShader_.SetMat4("model", model);
        modelRefractionShader_.SetMat4("transposeInverseModel", model.Inverse().Transpose());

        modelRefractionShader_.SetFloat("refractiveIndex", refractiveIndex_);
        modelRefractionShader_.SetFloat("refractionValue", refractionValue_);
        modelRefractionShader_.SetVec3("cameraPos", camera_.position);
        modelRefractionShader_.SetInt("skybox", 2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture_);
        mod->Draw(modelRefractionShader_);
        model = Mat4f::Identity;
        model = Transform3d::Translate(model, Vec3f::left * 2.0f);
        modelRefractionShader_.SetMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture_);
        cube_.Draw();
        break;
    }
    default:;
    }

    //Draw skybox
    glDepthFunc(GL_LEQUAL);
    skyboxShader_.Bind();
    skyboxShader_.SetMat4("view", Mat4f(view.ToMat3()));
    skyboxShader_.SetMat4("projection", projection);
    skyboxShader_.SetInt("skybox", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture_);
    skyboxCube_.Draw();
    glDepthFunc(GL_LESS);
}

void HelloCubemapsProgram::OnEvent(const SDL_Event& event)
{
    camera_.OnEvent(event);
}
}
