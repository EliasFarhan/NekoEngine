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

#include "26_hello_ssao/ssao_program.h"
#include "imgui.h"
#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif
namespace neko
{
void HelloSsaoProgram::Init()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Init SSAO Program");
#endif
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    glCheckError();
    CreateFramebuffer();
    //Crate white texture
    glGenTextures(1, &whiteTexture_);
    glBindTexture(GL_TEXTURE_2D, whiteTexture_);
    unsigned char white[] = { 255, 255, 255 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glCheckError();

    ssaoBlurShader_.LoadFromFile(
        config.dataRootPath + "shaders/26_hello_ssao/ssao.vert",
        config.dataRootPath + "shaders/26_hello_ssao/ssao_blur.frag");
    ssaoShader_.LoadFromFile(
        config.dataRootPath + "shaders/26_hello_ssao/ssao.vert",
        config.dataRootPath + "shaders/26_hello_ssao/ssao.frag");
    ssaoGeometryShader_.LoadFromFile(
        config.dataRootPath + "shaders/26_hello_ssao/ssao_geometry.vert",
        config.dataRootPath + "shaders/26_hello_ssao/ssao_geometry.frag");
    ssaoLightingShader_.LoadFromFile(
        config.dataRootPath + "shaders/26_hello_ssao/ssao.vert",
        config.dataRootPath + "shaders/26_hello_ssao/ssao_lighting.frag");
    // generate sample kernel
    ssaoKernel_.clear();
    ssaoKernel_.reserve(maxKernelSize_);
    for (int i = 0; i < maxKernelSize_; ++i)
    {
        Vec3f sample(
            RandomRange(-1.0f, 1.0f),
            RandomRange(-1.0f, 1.0f),
            RandomRange(0.0f, 1.0f));
        sample = sample.Normalized();
        sample *= RandomRange(0.0f, 1.0f);
        float scale = float(i) / 64.0f;

        // scale samples s.t. they're more aligned to center of kernel
        scale = Lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel_.push_back(sample);
    }
    // generate noise texture
    std::array<Vec3f, 16> ssaoNoise;
    for (auto& noise : ssaoNoise)
    {
        const Vec3f noiseValue = Vec3f(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f), 0.0f);
        noise = noiseValue;
    }
    glGenTextures(1, &noiseTexture_);
    glBindTexture(GL_TEXTURE_2D, noiseTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glCheckError();
    plane_.Init();
    screenPlane_.Init();
    modelId_ = modelManager_.LoadModel(config.dataRootPath + "model/nanosuit2/nanosuit.obj");
    glCheckError();
    camera_.Init();
}

void HelloSsaoProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    camera_.SetAspect(config.windowSize.x, config.windowSize.y);
    camera_.Update(dt);
}

void HelloSsaoProgram::Destroy()
{
    DestroyFramebuffer();
    glDeleteTextures(1, &whiteTexture_);
    glDeleteTextures(1, &noiseTexture_);
    ssaoBlurShader_.Destroy();
    ssaoGeometryShader_.Destroy();
    ssaoShader_.Destroy();
    ssaoLightingShader_.Destroy();
    plane_.Destroy();
    screenPlane_.Destroy();
    modelManager_.Destroy();
}

void HelloSsaoProgram::DrawImGui()
{
    ImGui::Begin("SSAO Program");
    ImGui::SliderFloat("Radius", &ssaoRadius_, 0.1f, 0.7f);
    ImGui::SliderFloat("Bias", &ssaoBias_, 0.005f, 0.05f);
    ImGui::SliderInt("Kernel Size", &kernelSize_, 1, maxKernelSize_);
    bool enableSsao = flags_ & ENABLE_SSAO;
    if (ImGui::Checkbox("Enable SSAO", &enableSsao))
    {
        flags_ = enableSsao ? flags_ | ENABLE_SSAO : flags_ & ~ENABLE_SSAO;
    }
    ImGui::End();
}

void HelloSsaoProgram::Render()
{
    if (!modelManager_.IsLoaded(modelId_))
    {
        return;
    }
    std::lock_guard<std::mutex> lock(updateMutex_);
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Render SSAO Program");
#endif
    if (flags_ & RESIZE_SCREEN)
    {
        DestroyFramebuffer();
        CreateFramebuffer();
        flags_ = flags_ & ~RESIZE_SCREEN;
    }
    const auto view = camera_.GenerateViewMatrix();
    const auto projection = camera_.GenerateProjectionMatrix();

    // 1. geometry pass: render scene's geometry/color data into gbuffer
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Geometry Pass");
#endif
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ssaoGeometryShader_.Bind();

    ssaoGeometryShader_.SetMat4("view", view);
    ssaoGeometryShader_.SetMat4("projection", projection);
    RenderScene(ssaoGeometryShader_);

    // 2. generate SSAO texture
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
    EASY_BLOCK("Generate SSAO Texture");
#endif
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFbo_);
    glClear(GL_COLOR_BUFFER_BIT);
    ssaoShader_.Bind();
    for (unsigned int i = 0; i < 64; i++)
    {
        ssaoShader_.SetVec3("samples[" + std::to_string(i) + "]", ssaoKernel_[i]);
    }
    ssaoShader_.SetMat4("projection", projection);
    ssaoShader_.SetTexture("gPosition", gPosition_, 0);
    ssaoShader_.SetTexture("gNormal", gNormal_, 1);
    ssaoShader_.SetTexture("texNoise", noiseTexture_, 2);

    ssaoShader_.SetInt("kernelSize", kernelSize_);
    ssaoShader_.SetFloat("radius", ssaoRadius_);
    ssaoShader_.SetFloat("bias", ssaoBias_);
    screenPlane_.Draw();

    // 3. blur SSAO texture to remove noise
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
    EASY_BLOCK("Blur SSAO Texture");
#endif
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFbo_);
    glClear(GL_COLOR_BUFFER_BIT);
    ssaoBlurShader_.Bind();
    ssaoBlurShader_.SetTexture("ssaoInput", ssaoColorBuffer_, 0);
    screenPlane_.Draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 4. lighting pass: traditional deferred Blinn-Phong lighting with added screen-space ambient occlusion
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
    EASY_BLOCK("Lighting pass");
#endif
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ssaoLightingShader_.Bind();
    const auto lightPosView = Vec3f(view * Vec4f(light_.position, 1.0f));
    ssaoLightingShader_.SetVec3("light.position", lightPosView);
    ssaoLightingShader_.SetVec3("light.color", light_.color);
    ssaoLightingShader_.SetFloat("light.linear", light_.linear);
    ssaoLightingShader_.SetFloat("light.quadratic", light_.quadratic);
    ssaoLightingShader_.SetFloat("light.constant", light_.constant);
    ssaoLightingShader_.SetTexture("gPosition", gPosition_, 0);
    ssaoLightingShader_.SetTexture("gNormal", gNormal_, 1);
    ssaoLightingShader_.SetTexture("gAlbedo", gAlbedoSpec_, 2);
    ssaoLightingShader_.SetTexture("ssao", ssaoColorBufferBlur_, 3);
    ssaoLightingShader_.SetBool("enableSSAO", flags_ & ENABLE_SSAO);
    screenPlane_.Draw();



}

void HelloSsaoProgram::OnEvent(const SDL_Event& event)
{
    camera_.OnEvent(event);
    if (event.type == SDL_WINDOWEVENT)
    {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            flags_ = flags_ | RESIZE_SCREEN;
        }
    }
}

void HelloSsaoProgram::DestroyFramebuffer()
{
    glDeleteFramebuffers(1, &gBuffer_);
    glDeleteTextures(1, &gPosition_);
    glDeleteTextures(1, &gNormal_);
    glDeleteTextures(1, &gAlbedoSpec_);
    glDeleteRenderbuffers(1, &rbo_);

    glDeleteFramebuffers(1, &ssaoFbo_);
    glDeleteFramebuffers(1, &ssaoBlurFbo_);
    glDeleteTextures(1, &ssaoColorBuffer_);
    glDeleteTextures(1, &ssaoColorBufferBlur_);

}

void HelloSsaoProgram::CreateFramebuffer()
{
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    glCheckError();
    glGenFramebuffers(1, &gBuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer_);

    // - position color buffer
    glGenTextures(1, &gPosition_);
    glBindTexture(GL_TEXTURE_2D, gPosition_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, config.windowSize.x, config.windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition_, 0);
    glCheckError();
    // - normal color buffer
    glGenTextures(1, &gNormal_);
    glBindTexture(GL_TEXTURE_2D, gNormal_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, config.windowSize.x, config.windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal_, 0);
    glCheckError();
    // - color + specular color buffer
    glGenTextures(1, &gAlbedoSpec_);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, config.windowSize.x, config.windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec_, 0);
    glCheckError();
    // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, config.windowSize.x, config.windowSize.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //Bind depth-stencil RBO to screen FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);
    glCheckError();
    glCheckFramebuffer();
    glCheckError();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // also create framebuffer to hold SSAO processing stage 
    // -----------------------------------------------------

    glGenFramebuffers(1, &ssaoFbo_);  glGenFramebuffers(1, &ssaoBlurFbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFbo_);

    // SSAO color buffer
    glGenTextures(1, &ssaoColorBuffer_);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, config.windowSize.x, config.windowSize.y, 0, GL_RED, GL_FLOAT, NULL);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer_, 0);
    const GLenum colorAttachment = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &colorAttachment);
    glCheckFramebuffer();
    glCheckError();
    // and blur stage
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFbo_);
    glGenTextures(1, &ssaoColorBufferBlur_);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, config.windowSize.x, config.windowSize.y, 0, GL_RED, GL_FLOAT, NULL);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur_, 0);
    glDrawBuffers(1, &colorAttachment);
    glCheckFramebuffer();
    glCheckError();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
}

void HelloSsaoProgram::RenderScene(const gl::Shader& shader)
{
    const auto view = camera_.GenerateViewMatrix();

    //Draw floor
    auto model = Mat4f::Identity;
    model = Transform3d::Rotate(model, Quaternion::AngleAxis(degree_t(-90.0f), Vec3f::right));
    model = Transform3d::Scale(model, Vec3f::one * 5.0f);
    //model = Transform3d::Translate(model, Vec3f::forward * camera_.farPlane / 2.0f);
    shader.SetMat4("model", model);
    shader.SetMat4("normalMatrix", (view * model).Inverse().Transpose());
    plane_.Draw();
    //Draw model
    model = Mat4f::Identity;
    model = model = Transform3d::Rotate(model, degree_t(90.0f), Vec3f::right);
    model = Transform3d::Scale(model, Vec3f::one * 0.1f);
    model = Transform3d::Translate(model, Vec3f::up * 0.1f);
    shader.SetMat4("model", model);
    shader.SetMat4("normalMatrix", (view * model).Inverse().Transpose());
    auto* mod = modelManager_.GetModel(modelId_);
    mod->Draw(shader);

    glCheckError();
}
}
