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

#include "gl/shape.h"
#include "11_hello_framebuffer/framebuffer_program.h"
#include "imgui.h"

namespace neko
{


HelloFramebufferProgram::HelloFramebufferProgram() :
    screenFrame_(gl::RenderQuad(Vec3f::zero, Vec2f(2.0f,2.0f)))
{

}

void HelloFramebufferProgram::Init()
{
    textureManager_.Init();
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    screenFrame_.Init();
    cube_.Init();
	//Create Screen FBO
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    //Generate Color Buffer for FBO
    glGenTextures(1, &fboColorBufferTexture_);
    glBindTexture(GL_TEXTURE_2D, fboColorBufferTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, config.windowSize.x, config.windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    //Bind the Color Buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboColorBufferTexture_, 0);
    //Generate the Depth-Stencil RenderBuffer Object
    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, config.windowSize.x, config.windowSize.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//Bind depth-stencil RBO to screen FBO 
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        logDebug("[Error] Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    screenShader_.LoadFromFile(
            config.dataRootPath + "shaders/11_hello_framebuffer/screen.vert",
            config.dataRootPath + "shaders/11_hello_framebuffer/screen.frag");
    screenInverseShader_.LoadFromFile(
            config.dataRootPath + "shaders/11_hello_framebuffer/screen.vert",
            config.dataRootPath + "shaders/11_hello_framebuffer/screen_inverse.frag");
    screenGrayscaleShader_.LoadFromFile(
            config.dataRootPath + "shaders/11_hello_framebuffer/screen.vert",
            config.dataRootPath + "shaders/11_hello_framebuffer/screen_grayscale.frag");
    screenBlurShader_.LoadFromFile(
            config.dataRootPath + "shaders/11_hello_framebuffer/screen.vert",
            config.dataRootPath + "shaders/11_hello_framebuffer/screen_blur.frag");
    screenEdgeDetectionShader_.LoadFromFile(
            config.dataRootPath + "shaders/11_hello_framebuffer/screen.vert",
            config.dataRootPath + "shaders/11_hello_framebuffer/screen_edge_detection.frag");
    containerTextureId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/container.jpg", gl::Texture::DEFAULT);


    modelShader_.LoadFromFile(config.dataRootPath + "shaders/11_hello_framebuffer/model.vert",
                              config.dataRootPath + "shaders/11_hello_framebuffer/model.frag");
    camera_.position = Vec3f(0.0f,-5.0f,-5.0f);
    camera_.WorldLookAt(Vec3f::zero);
}

void HelloFramebufferProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    camera_.SetAspect(config.windowSize.x, config.windowSize.y);
    camera_.Update(dt);	textureManager_.Update(dt);
}

void HelloFramebufferProgram::Destroy()
{
    textureManager_.Destroy();

    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(1, &fboColorBufferTexture_);
    glDeleteRenderbuffers(1, &rbo_);

    cube_.Destroy();
    screenFrame_.Destroy();

    modelShader_.Destroy();
    screenShader_.Destroy();
    screenGrayscaleShader_.Destroy();
    screenInverseShader_.Destroy();
    screenEdgeDetectionShader_.Destroy();
    screenBlurShader_.Destroy();
}

void HelloFramebufferProgram::DrawImGui()
{
    ImGui::Begin("Post Processing");
    const char* postProcessingNames[(int)PostProcessingType::LENGTH] =
    {
        "No Processing",
        "Inverse",
        "Grayscale",
        "Blur",
        "Edge Detection"
    };
    int currentIndex = (int)postProcessingType_;
    if(ImGui::Combo("Post Processing Type", &currentIndex, postProcessingNames, (int)PostProcessingType::LENGTH))
    {
        postProcessingType_ = (PostProcessingType)currentIndex;
    }
    ImGui::End();
}

void HelloFramebufferProgram::Render()
{

    if (containerTexture_ == gl::INVALID_TEXTURE_NAME)
    {
        containerTexture_ = textureManager_.GetTextureName(containerTextureId_);
        return;
    }
    std::lock_guard<std::mutex> lock(updateMutex_);
    if(hasScreenResize_)
    {
        //When the screen resize, we need to resize
        glDeleteFramebuffers(1, &fbo_);
        glDeleteTextures(1, &fboColorBufferTexture_);
        glDeleteRenderbuffers(1, &rbo_);

        const auto& config = BasicEngine::GetInstance()->GetConfig();
        glGenFramebuffers(1, &fbo_);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

        glGenTextures(1, &fboColorBufferTexture_);
        glBindTexture(GL_TEXTURE_2D, fboColorBufferTexture_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, config.windowSize.x, config.windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboColorBufferTexture_, 0);

        glGenRenderbuffers(1, &rbo_);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, config.windowSize.x, config.windowSize.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            logDebug("[Error] Framebuffer is not complete afetr resize!");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        hasScreenResize_ = false;

        logDebug("Framebuffer resized with size: "+std::to_string(config.windowSize.x)+", "+std::to_string(config.windowSize.y));
    }

    //Bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //Draw scene
    modelShader_.Bind();
    modelShader_.SetMat4("model", Mat4f::Identity);
    modelShader_.SetMat4("view", camera_.GenerateViewMatrix());
    modelShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());

    modelShader_.SetInt("texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, containerTexture_);
    cube_.Draw();

    //Bind backbuffer
    gl::Shader* currentShader = nullptr;
    switch (postProcessingType_)
    {
        case PostProcessingType::NO_POSTPROCESS:
            currentShader = &screenShader_;
            break;
        case PostProcessingType::INVERSE:
            currentShader = &screenInverseShader_;
            break;
        case PostProcessingType::GRAYSCALE:
            currentShader = &screenGrayscaleShader_;
            break;
        case PostProcessingType::BLUR:
            currentShader = &screenBlurShader_;
            break;
        case PostProcessingType::EDGE_DETECTION:
            currentShader = &screenEdgeDetectionShader_;
            break;
        default:
            break;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    currentShader->Bind();
    glDisable(GL_DEPTH_TEST);
    currentShader->SetInt("screenTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboColorBufferTexture_);
    screenFrame_.Draw();


}
void HelloFramebufferProgram::OnEvent(const SDL_Event& event)
{
    camera_.OnEvent(event);
    if (event.type == SDL_WINDOWEVENT)
    {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            hasScreenResize_ = true;
        }
    }
}



}