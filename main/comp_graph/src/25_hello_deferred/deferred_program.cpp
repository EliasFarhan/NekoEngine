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
#include "25_hello_deferred/deferred_progam.h"
#include "imgui.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

namespace neko
{

void HelloDeferredProgram::Init()
{
    textureManager_.Init();
    const auto& config = BasicEngine::GetInstance()->config;
    glCheckError();
    floor_.Init();
    screenQuad_.Init();
    CreateFramebuffer();

    deferredShader_.LoadFromFile(
            config.dataRootPath+"shaders/25_hello_deferred/deferred.vert",
            config.dataRootPath+"shaders/25_hello_deferred/deferred.frag"
            );
    lightingShader_.LoadFromFile(
            config.dataRootPath+"shaders/25_hello_deferred/lighting.vert",
            config.dataRootPath+"shaders/25_hello_deferred/lighting.frag"
    );
    forwardShader_.LoadFromFile(
            config.dataRootPath+"shaders/25_hello_deferred/forward.vert",
            config.dataRootPath+"shaders/25_hello_deferred/forward.frag"
    );

    glGenTextures(1, &whiteTexture_);
    glBindTexture(GL_TEXTURE_2D, whiteTexture_);
    unsigned char white[] = {255, 255, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glCheckError();

    model_.LoadModel(config.dataRootPath+"model/nanosuit2/nanosuit.obj");
    cube_.Init();
    containerId_ = textureManager_.LoadTexture(config.dataRootPath + "material/container2.png");
    containerSpecularId_ = textureManager_.LoadTexture(config.dataRootPath + "material/container2_specular.png");


    camera_.position = Vec3f(0.0f, 3.0f, -3.0f);
    camera_.WorldLookAt(Vec3f::zero);


    for(int x = -2; x < 3; x++)
    {
        for(int z = -2; z < 3; z++)
        {
            lights_[(x+2)+(z+2)*5].color = Vec3f(RandomRange(0.0f,1.0f), RandomRange(0.0f,1.0f), RandomRange(0.0f,1.0f));
            lights_[(x+2)+(z+2)*5].position = Vec3f(2.5f*float(x), 1.5f, 2.5f*(float(z)+2.0f));
        }
    }
    glCheckError();
}

void HelloDeferredProgram::Update(seconds dt)
{

    std::lock_guard<std::mutex> lock(updateMutex_);
    const auto& config = BasicEngine::GetInstance()->config;
    camera_.SetAspect(config.windowSize.x, config.windowSize.y);
    camera_.Update(dt);	textureManager_.Update(dt);

}

void HelloDeferredProgram::Destroy()
{
    glDeleteFramebuffers(1, &gBuffer_);
    glDeleteTextures(1, &gPosition_);
    glDeleteTextures(1, &gNormal_);
    glDeleteTextures(1, &gAlbedoSpec_);
    floor_.Destroy();
    screenQuad_.Destroy();
    model_.Destroy();
    cube_.Destroy();
    glDeleteBuffers(1, &rbo_);

    glDeleteTextures(1, &whiteTexture_);

    textureManager_.Destroy();
}

void HelloDeferredProgram::DrawImGui()
{
    ImGui::Begin("Deferred Program");
    bool enableForward = flags_ & FORWARD_RENDERING;
    if(ImGui::Checkbox("Enable Forward", &enableForward))
    {
        flags_ = enableForward? flags_ | FORWARD_RENDERING : flags_ & ~FORWARD_RENDERING;
    }
    ImGui::End();
}

void HelloDeferredProgram::Render()
{
    if(!model_.IsLoaded())
    {
        return;
    }
    if (containerSpecular_ == INVALID_TEXTURE_NAME)
    {
        containerSpecular_ = textureManager_.GetTexture(containerSpecularId_).name;
        if (containerSpecular_ == INVALID_TEXTURE_NAME)
			return;
    }
    if(container_ == INVALID_TEXTURE_NAME)
    {
        container_ = textureManager_.GetTexture(containerId_).name;
        if (container_ == INVALID_TEXTURE_NAME)
            return;
        return;
    }

    std::lock_guard<std::mutex> lock(updateMutex_);
    glCheckError();
    if(flags_ & RESIZE_SCREEN)
    {
        glDeleteFramebuffers(1, &gBuffer_);
        glDeleteTextures(1, &gPosition_);
        glDeleteTextures(1, &gNormal_);
        glDeleteTextures(1, &gAlbedoSpec_);

        glDeleteBuffers(1, &rbo_);
        CreateFramebuffer();
        flags_ = flags_ & ~RESIZE_SCREEN;
    }

    if(flags_ & FORWARD_RENDERING)
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Forward Rendering");
#endif
        forwardShader_.Bind();
        forwardShader_.SetMat4("view", camera_.GenerateViewMatrix());
        forwardShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
        forwardShader_.SetVec3("viewPos", camera_.position);
        for(int i = 0; i < 32; i++)
        {
            forwardShader_.SetVec3("lights["+std::to_string(i)+"].position", lights_[i].position);
            forwardShader_.SetVec3("lights["+std::to_string(i)+"].color", lights_[i].color);
        }
        RenderScene(forwardShader_);
    }
    else
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Deferred Rendering");
#endif
        //G-Buffer pass
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer_);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        deferredShader_.Bind();
        deferredShader_.SetMat4("view", camera_.GenerateViewMatrix());
        deferredShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());

        RenderScene(deferredShader_);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        lightingShader_.Bind();
        for(int i = 0; i < 32; i++)
        {
            lightingShader_.SetVec3("lights["+std::to_string(i)+"].position", lights_[i].position);
            lightingShader_.SetVec3("lights["+std::to_string(i)+"].color", lights_[i].color);
        }
        lightingShader_.SetTexture("gPosition", gPosition_, 0);
        lightingShader_.SetTexture("gNormal", gNormal_, 1);
        lightingShader_.SetTexture("gAlbedoSpec", gAlbedoSpec_, 2);
        lightingShader_.SetVec3("viewPos", camera_.position);
        screenQuad_.Draw();
    }
}

void HelloDeferredProgram::OnEvent(const SDL_Event& event)
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

void HelloDeferredProgram::CreateFramebuffer()
{
	
    const auto& config = BasicEngine::GetInstance()->config;
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

// - normal color buffer
    glGenTextures(1, &gNormal_);
    glBindTexture(GL_TEXTURE_2D, gNormal_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, config.windowSize.x, config.windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal_, 0);

// - color + specular color buffer
    glGenTextures(1, &gAlbedoSpec_);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, config.windowSize.x, config.windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec_, 0);

    // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, config.windowSize.x, config.windowSize.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //Bind depth-stencil RBO to screen FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

    glCheckFramebuffer();


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
}

void HelloDeferredProgram::RenderScene(const gl::Shader& shader)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("�Render Scene");
#endif
    for(int x = -2; x < 3; x++)
    {
        for(int z = -2; z < 3; z++)
        {
            shader.SetTexture("material.texture_diffuse1", container_, 0);
            shader.SetTexture("material.texture_specular1", containerSpecular_, 1);
            auto model = Mat4f::Identity;
            model = Transform3d::Translate(model, Vec3f(2.0f*float(x), 0.0f, 2.0f*(float(z)+2.0f)));
            shader.SetMat4("model", model);
            shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
            cube_.Draw();
            model = Mat4f::Identity;
            model = Transform3d::Scale(model, Vec3f::one*0.2f);
            model = Transform3d::Translate(model, Vec3f(2.0f*(float(x)+0.5f), 0.0f, 2.0f*(float(z)+2.5f)));
            shader.SetMat4("model", model);
            shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
            model_.Draw(shader);
        }
    }
    auto model = Mat4f::Identity;
    model = Transform3d::Rotate(model, Quaternion::AngleAxis(degree_t(-90.0f), Vec3f::right));
    model = Transform3d::Scale(model, Vec3f::one * camera_.farPlane);
    model = Transform3d::Translate(model, Vec3f::forward * camera_.farPlane / 2.0f);
    shader.SetTexture("material.texture_diffuse1", whiteTexture_, 0);
    shader.SetTexture("material.texture_specular1", whiteTexture_, 1);
    shader.SetMat4("model", model);
    shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
    floor_.Draw();
}
}