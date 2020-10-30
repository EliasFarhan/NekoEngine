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

#include "19_hello_hdr/hdr_program.h"
#include "imgui.h"

namespace neko
{


void HelloHdrProgram::Init()
{
    textureManager_.Init();
    const auto& config = BasicEngine::GetInstance()->config;
    cube_.Init();
    cubeShader_.LoadFromFile(
            config.dataRootPath + "shaders/19_hello_hdr/tunnel.vert",
            config.dataRootPath + "shaders/19_hello_hdr/tunnel.frag");

    cubeTextureId_ = textureManager_.LoadTexture(
        config.dataRootPath+"sprites/brickwall/brickwall.jpg",
        Texture::TextureFlags(Texture::MIRROR_REPEAT_WRAP | Texture::GAMMA_CORRECTION | Texture::DEFAULT));


    CreateFramebuffer();
    
    hdrPlane_.Init();
    hdrShader_.LoadFromFile(
            config.dataRootPath + "shaders/19_hello_hdr/hdr_screen.vert",
            config.dataRootPath + "shaders/19_hello_hdr/hdr_screen.frag");
    camera_.position = Vec3f::zero;
    camera_.WorldLookAt(camera_.position+Vec3f::forward);

}

void HelloHdrProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    const auto& config = BasicEngine::GetInstance()->config;
    camera_.SetAspect(config.windowSize.x, config.windowSize.y);
    camera_.Update(dt);	textureManager_.Update(dt);
}

void HelloHdrProgram::Destroy()
{
    textureManager_.Destroy();
    cubeShader_.Destroy();
    cube_.Destroy();

    glDeleteFramebuffers(1, &hdrFbo_);
    glDeleteTextures(1, &hdrColorBuffer_);
    glDeleteRenderbuffers(1, &hdrRbo_);
}

void HelloHdrProgram::DrawImGui()
{
    ImGui::Begin("Hdr Program");
    bool enableHdr = flags_ & ENABLE_HDR;
    if(ImGui::Checkbox("Enable Hdr", &enableHdr))
    {
        flags_ = enableHdr ? flags_ | ENABLE_HDR : flags_ & ~ENABLE_HDR;
    }
    ImGui::SliderFloat("Exposure", &exposure_, 0.1f, 6.0f);
    ImGui::End();
}

void HelloHdrProgram::Render()
{
    if (cubeTexture_ == INVALID_TEXTURE_NAME)
    {
        cubeTexture_ = textureManager_.GetTexture(cubeTextureId_).name;
        return;
    }
	if(flags_ & RESIZE_FRAMEBUFFER)
	{
        glDeleteFramebuffers(1, &hdrFbo_);
        glDeleteTextures(1, &hdrColorBuffer_);
        glDeleteRenderbuffers(1, &hdrRbo_);
        CreateFramebuffer();
        flags_ = flags_ & ~RESIZE_FRAMEBUFFER;
    }   
		
	
    std::lock_guard<std::mutex> lock(updateMutex_);

    glBindFramebuffer(GL_FRAMEBUFFER, hdrFbo_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cubeShader_.Bind();
    cubeShader_.SetMat4("view", camera_.GenerateViewMatrix());
    cubeShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
    cubeShader_.SetMat4("model", Mat4f::Identity);
    cubeShader_.SetMat4("transposeInverseModel", Mat4f::Identity);
    cubeShader_.SetInt("diffuseTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture_);
    for(size_t i = 0; i < lights_.size(); i++)
    {
        cubeShader_.SetVec3("lights["+std::to_string(i)+"].Position", lights_[i].lightPos_);
        cubeShader_.SetVec3("lights["+std::to_string(i)+"].Color", lights_[i].lightColor_);
    }
    cubeShader_.SetInt("lightNmb", lights_.size());
    cubeShader_.SetBool("inverseNormals", true);
    cube_.Draw();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    hdrShader_.Bind();
    hdrShader_.SetTexture("hdrBuffer", hdrColorBuffer_);
    hdrShader_.SetBool("hdr", flags_ & ENABLE_HDR);
    hdrShader_.SetFloat("exposure", exposure_);
    hdrPlane_.Draw();

}

void HelloHdrProgram::OnEvent(const SDL_Event& event)
{
    camera_.OnEvent(event);
    if (event.type == SDL_WINDOWEVENT)
    {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            flags_ = flags_ | RESIZE_FRAMEBUFFER;
        }
    }
}

void HelloHdrProgram::CreateFramebuffer()
{
    const auto& config = BasicEngine::GetInstance()->config;
    glGenFramebuffers(1, &hdrFbo_);
    // create floating point color buffer
    glGenTextures(1, &hdrColorBuffer_);
    glBindTexture(GL_TEXTURE_2D, hdrColorBuffer_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, config.windowSize.x, config.windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // create depth buffer (renderbuffer)
    glGenRenderbuffers(1, &hdrRbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, hdrRbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, config.windowSize.x, config.windowSize.y);
    // attach buffers
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrColorBuffer_, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, hdrRbo_);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        logDebug("[Error] Framebuffer not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
}
}
