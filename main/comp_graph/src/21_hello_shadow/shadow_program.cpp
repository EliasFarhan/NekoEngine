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

#include "21_hello_shadow/shadow_program.h"
#include "imgui.h"

namespace neko
{


void HelloShadowProgram::Init()
{
    textureManager_.Init();
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    glCheckError();
    cube_.Init();
    floor_.Init();

    modelId_ = modelManager_.LoadModel(config.dataRootPath + "model/nanosuit2/nanosuit.obj");

    floorTextureId_ = textureManager_.LoadTexture(
        config.dataRootPath + "sprites/brickwall/brickwall.jpg", Texture::DEFAULT);

    glGenFramebuffers(1, &depthMapFbo_);
    glGenTextures(1, &depthMap_);
    glBindTexture(GL_TEXTURE_2D, depthMap_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
        GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,
        GL_LEQUAL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap_, 0);
    GLenum drawBuffers = GL_NONE;
    glDrawBuffers(1, &drawBuffers);
    glReadBuffer(GL_NONE);

    glCheckFramebuffer();
    glCheckError();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    depthCamera_.SetExtends(Vec2f::one * 4.0f);
    depthCamera_.position = light_.lightPos;
    depthCamera_.WorldLookAt(light_.lightPos + light_.lightDir);


    camera_.position = Vec3f(0, 3, 3);
    camera_.WorldLookAt(Vec3f::zero);

    simpleDepthShader_.LoadFromFile(config.dataRootPath + "shaders/21_hello_shadow/simple_depth.vert",
        config.dataRootPath + "shaders/21_hello_shadow/simple_depth.frag");
    modelShader_.LoadFromFile(config.dataRootPath + "shaders/21_hello_shadow/shadow.vert",
        config.dataRootPath + "shaders/21_hello_shadow/shadow.frag");
    glCheckError();
}

void HelloShadowProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    camera_.SetAspect(config.windowSize.x, config.windowSize.y);
    camera_.Update(dt);
    textureManager_.Update(dt);
    modelManager_.Update(dt);
}

void HelloShadowProgram::Destroy()
{
    cube_.Destroy();
    floor_.Destroy();
    modelManager_.Destroy();

    modelShader_.Destroy();
    simpleDepthShader_.Destroy();

    textureManager_.Destroy();

    glDeleteFramebuffers(1, &depthMapFbo_);
    glDeleteTextures(1, &depthMap_);
    glDisable(GL_CULL_FACE);

}


void HelloShadowProgram::DrawImGui()
{
    ImGui::Begin("Shadow Program");
    if (ImGui::InputFloat3("Light Pos", &light_.lightPos[0]))
    {
        depthCamera_.position = light_.lightPos;
    }
    bool enableShadow = flags_ & ENABLE_SHADOW;
    if (ImGui::Checkbox("Enable Shadow", &enableShadow))
    {
        flags_ = enableShadow ? flags_ | ENABLE_SHADOW : flags_ & ~ENABLE_SHADOW;
    }
    bool enableBias = flags_ & ENABLE_BIAS;
    if (ImGui::Checkbox("Enable Bias", &enableBias))
    {
        flags_ = enableBias ? flags_ | ENABLE_BIAS : flags_ & ~ENABLE_BIAS;
    }
    ImGui::InputFloat("Shadow Bias", &shadowBias_, 0, 0, "%.6f");
    bool enablePeterPanning = flags_ & ENABLE_PETER_PANNING;
    if (ImGui::Checkbox("Peter Panning", &enablePeterPanning))
    {
        flags_ = enablePeterPanning ? flags_ | ENABLE_PETER_PANNING : flags_ & ~ENABLE_PETER_PANNING;
    }
    bool enableOverSampling = flags_ & ENABLE_OVER_SAMPLING;
    if (ImGui::Checkbox("Over Sampling", &enableOverSampling))
    {
        flags_ = enableOverSampling ? flags_ | ENABLE_OVER_SAMPLING : flags_ & ~ENABLE_OVER_SAMPLING;
    }
    Vec2f depthCameraSize = Vec2f(depthCamera_.right, depthCamera_.top);
    if (ImGui::InputFloat2("Depthmap Size", &depthCameraSize[0]))
    {
        depthCamera_.SetExtends(depthCameraSize);
    }
    bool enablePcf = flags_ & ENABLE_PCF;
    if (ImGui::Checkbox("Enable PCF", &enablePcf))
    {
        flags_ = enablePcf ? flags_ | ENABLE_PCF : flags_ & ~ENABLE_PCF;
    }

    ImGui::End();
}

void HelloShadowProgram::Render()
{
    if (!modelManager_.IsLoaded(modelId_))
    {
        return;
    }
    if (floorTexture_ == INVALID_TEXTURE_NAME)
    {
        floorTexture_ = textureManager_.GetTextureName(floorTextureId_);
        return;
    }
    std::lock_guard<std::mutex> lock(updateMutex_);
    glCheckError();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    const auto lightView = depthCamera_.GenerateViewMatrix();
    const auto lightProjection = depthCamera_.GenerateProjectionMatrix();
    const auto lightSpaceMatrix = lightProjection * lightView;
    if (flags_ & ENABLE_SHADOW)
    {
        //Render depth buffer from light
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo_);
        glClear(GL_DEPTH_BUFFER_BIT);
        simpleDepthShader_.Bind();

        simpleDepthShader_.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
        if (flags_ & ENABLE_PETER_PANNING)
        {
            glCullFace(GL_FRONT);
        }
        RenderScene(simpleDepthShader_);
        if (flags_ & ENABLE_PETER_PANNING)
        {
            glCullFace(GL_BACK);
        }
        //Render scene with shadow
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCheckError();
        glViewport(0, 0, config.windowSize.x, config.windowSize.y);
    }
    modelShader_.Bind();
    modelShader_.SetMat4("view", camera_.GenerateViewMatrix());
    modelShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
    modelShader_.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
    modelShader_.SetTexture("shadowMap", depthMap_, 3);
    modelShader_.SetBool("enableBias", flags_ & ENABLE_BIAS);
    modelShader_.SetBool("enableShadow", flags_ & ENABLE_SHADOW);
    modelShader_.SetBool("enableOverSampling", flags_ & ENABLE_OVER_SAMPLING);
    modelShader_.SetBool("enablePcf", flags_ & ENABLE_PCF);
    modelShader_.SetVec3("viewPos", camera_.position);
    modelShader_.SetFloat("bias", shadowBias_);
    modelShader_.SetVec3("light.lightDir", light_.lightDir);
    RenderScene(modelShader_);
    glCheckError();
}

void HelloShadowProgram::RenderScene(const gl::Shader& shader)
{
    //Render model
    auto model = Mat4f::Identity;
    model = Transform3d::Translate(model, Vec3f::forward * 5.0f);
    model = Transform3d::Scale(model, Vec3f(0.1f));
    shader.SetMat4("model", model);
    shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
    auto* mod = modelManager_.GetModel(modelId_);
    mod->Draw(shader);
    //Render floor
    model = Mat4f::Identity;
    model = Transform3d::Rotate(model, degree_t(-90.0f), Vec3f::right);
    shader.SetMat4("model", model);
    shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
    shader.SetTexture("material.texture_diffuse1", floorTexture_, 0);
    floor_.Draw();
    //Render cubes
    for (const auto& transform : cubeTransforms_)
    {
        model = Mat4f::Identity;
        model = Transform3d::Translate(model, transform.position);
        model = Transform3d::Rotate(model, transform.angle, transform.axis);
        model = Transform3d::Scale(model, transform.scale);
        shader.SetMat4("model", model);
        shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
        cube_.Draw();
    }
}


void HelloShadowProgram::OnEvent(const SDL_Event& event)
{
    camera_.OnEvent(event);
}
}