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

#include "18_hello_normal/normal_program.h"
#include "imgui.h"

namespace neko
{

void HelloNormalProgram::Init()
{
    textureManager_.Init();
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    diffuseTexId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/brickwall/brickwall.jpg", Texture::DEFAULT);

    normalTexId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/brickwall/brickwall_normal.jpg", Texture::DEFAULT);


    normalShader_.LoadFromFile(
        config.dataRootPath + "shaders/18_hello_normal/normal.vert",
        config.dataRootPath + "shaders/18_hello_normal/normal.frag");
    diffuseShader_.LoadFromFile(
        config.dataRootPath + "shaders/18_hello_normal/model.vert",
        config.dataRootPath + "shaders/18_hello_normal/model.frag");

    plane_.Init();
    cube_.Init();
    modelId_ = modelManager_.LoadModel(config.dataRootPath + "model/nanosuit2/nanosuit.obj");
    sphere_.Init();
    camera_.position = Vec3f(-3.0f, 3.0f, 3.0f);
    camera_.WorldLookAt(Vec3f::zero);
}

void HelloNormalProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    dt_ += dt.count();
    lightPos_ = Vec3f(Cos(radian_t(dt_)), 1.0f, Sin(radian_t(dt_))) * 3.0f;
    camera_.SetAspect(config.windowSize.x, config.windowSize.y);
    camera_.Update(dt);	textureManager_.Update(dt);
    modelManager_.Update(dt);
}

void HelloNormalProgram::Destroy()
{
    modelManager_.Destroy();
    normalShader_.Destroy();
    plane_.Destroy();
    cube_.Destroy();
    textureManager_.Destroy();
    sphere_.Destroy();
}

void HelloNormalProgram::DrawImGui()
{
    ImGui::Begin("Normal Program");
    bool useModel = flags_ & ENABLE_MODEL;
    if (ImGui::Checkbox("Use Model", &useModel))
    {
        flags_ = useModel ? flags_ | ENABLE_MODEL : flags_ & ~ENABLE_MODEL;
    }
    bool usePlane = flags_ & ENABLE_PLANE;
    if (ImGui::Checkbox("Use Plane", &usePlane))
    {
        flags_ = usePlane ? flags_ | ENABLE_PLANE : flags_ & ~ENABLE_PLANE;
    }
    bool useCube = flags_ & ENABLE_CUBE;
    if (ImGui::Checkbox("Use Cube", &useCube))
    {
        flags_ = useCube ? flags_ | ENABLE_CUBE : flags_ & ~ENABLE_CUBE;
    }
    bool enableSphere = flags_ & ENABLE_SPHERE;
    if (ImGui::Checkbox("Enable Sphere", &enableSphere))
    {
        flags_ = enableSphere ? flags_ | ENABLE_SPHERE : flags_ & ENABLE_SPHERE;
    }
    bool enableNormal = flags_ & ENABLE_NORMAL_MAP;
    if (ImGui::Checkbox("Enable Normal Map", &enableNormal))
    {
        flags_ = enableNormal ? flags_ | ENABLE_NORMAL_MAP : flags_ & ~ENABLE_NORMAL_MAP;
    }

    ImGui::End();
}

void HelloNormalProgram::Render()
{
    if (!modelManager_.IsLoaded(modelId_))
        return;
    if (normalTex_ == INVALID_TEXTURE_NAME)
    {
        normalTex_ = textureManager_.GetTextureName(normalTexId_);
        return;
    }
    if (diffuseTex_ == INVALID_TEXTURE_NAME)
    {
        diffuseTex_ = textureManager_.GetTextureName(diffuseTexId_);
        return;
    }
    std::lock_guard<std::mutex> lock(updateMutex_);

    const std::function<void(NormalFlags)> draw = [this](NormalFlags flag)
    {
        auto model = Mat4f::Identity;
        if (flag == ENABLE_MODEL)
        {
            model = Transform3d::Scale(model, Vec3f::one * 0.1f);
        }
        else
        {
            model = Transform3d::Scale(model, Vec3f::one * 3.0f);
        }
        const auto transposeInverseModel = model.Inverse().Transpose();
        if (flags_ & ENABLE_NORMAL_MAP)
        {
            normalShader_.Bind();
            normalShader_.SetVec3("viewPos", camera_.position);
            normalShader_.SetMat4("view", camera_.GenerateViewMatrix());
            normalShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
            normalShader_.SetVec3("lightPos", lightPos_);
            normalShader_.SetMat4("model", model);

            normalShader_.SetMat4("transposeInverseModel", transposeInverseModel);
            normalShader_.SetBool("enableNormalMap", true);
            if (flag != ENABLE_MODEL)
            {
                normalShader_.SetInt("material.texture_diffuse1", 0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, diffuseTex_);
                normalShader_.SetInt("material.texture_normal1", 1);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, normalTex_);
            }
        }
        else
        {
            diffuseShader_.Bind();
            diffuseShader_.SetVec3("viewPos", camera_.position);
            diffuseShader_.SetVec3("lightPos", lightPos_);

            diffuseShader_.SetMat4("view", camera_.GenerateViewMatrix());
            diffuseShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
            diffuseShader_.SetMat4("model", model);
            diffuseShader_.SetMat4("transposeInverseModel", transposeInverseModel);
            if (flag != ENABLE_MODEL)
            {
                diffuseShader_.SetInt("material.texture_diffuse1", 0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, diffuseTex_);
            }
        }
        switch (flag)
        {
        case ENABLE_PLANE:
            plane_.Draw();
            break;
        case ENABLE_MODEL:
        {
            auto* mod = modelManager_.GetModel(modelId_);
            mod->Draw(flags_ & ENABLE_NORMAL_MAP ? normalShader_ : diffuseShader_);
            break;
        }
        case ENABLE_CUBE:
            cube_.Draw();
            break;
        case ENABLE_SPHERE:
            sphere_.Draw();
            break;
        default:
            break;
        }
    };
    if (flags_ & ENABLE_MODEL)
    {
        draw(ENABLE_MODEL);
    }
    if (flags_ & ENABLE_CUBE)
    {
        draw(ENABLE_CUBE);
    }
    if (flags_ & ENABLE_PLANE)
    {
        draw(ENABLE_PLANE);
    }
    if (flags_ & ENABLE_SPHERE)
    {
        draw(ENABLE_SPHERE);
    }

}

void HelloNormalProgram::OnEvent(const SDL_Event& event)
{
    camera_.OnEvent(event);
}

}