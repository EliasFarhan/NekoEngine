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

#include "20_hello_bloom/bloom_program.h"
#include "imgui.h"

namespace neko
{


void HelloBloomProgram::Init()
{
    textureManager_.Init();
	const auto& config = BasicEngine::GetInstance()->GetConfig();
    cubeShader_.LoadFromFile(
            config.dataRootPath + "shaders/20_hello_bloom/cube.vert",
            config.dataRootPath + "shaders/20_hello_bloom/cube.frag");
    lightShader_.LoadFromFile(
            config.dataRootPath + "shaders/20_hello_bloom/cube.vert",
            config.dataRootPath + "shaders/20_hello_bloom/light_cube.frag");
    blurShader_.LoadFromFile(
            config.dataRootPath + "shaders/20_hello_bloom/blur.vert",
            config.dataRootPath + "shaders/20_hello_bloom/blur.frag");
    bloomShader_.LoadFromFile(
            config.dataRootPath + "shaders/20_hello_bloom/bloom.vert",
            config.dataRootPath + "shaders/20_hello_bloom/bloom.frag");

	cube_.Init();
    cubeTextureId_ = textureManager_.LoadTexture(
        config.dataRootPath + "sprites/container.jpg",
        Texture::TextureFlags(Texture::REPEAT_WRAP | Texture::SMOOTH_TEXTURE | Texture::GAMMA_CORRECTION));


    screenPlane_.Init();

    camera_.position = Vec3f::forward*5.0f;
    camera_.WorldLookAt(Vec3f::zero);
    
    CreateFramebuffer();
}

void HelloBloomProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->GetConfig();
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
	textureManager_.Update(dt);
}

void HelloBloomProgram::Destroy()
{
    textureManager_.Destroy();
	cubeShader_.Destroy();
	lightShader_.Destroy();
	blurShader_.Destroy();
	bloomShader_.Destroy();
	cube_.Destroy();
    screenPlane_.Destroy();

    glDeleteFramebuffers(1, &hdrFbo_);
    glDeleteFramebuffers(2, &pingpongFbo_[0]);

    glDeleteTextures(2, &colorBuffers_[0]);
    glDeleteTextures(2, &pingpongColorBuffers_[0]);

    glDeleteRenderbuffers(1, &rbo_);
}

void HelloBloomProgram::DrawImGui()
{
    ImGui::Begin("Bloom Program");
    bool enableBloom = flags_ & ENABLE_BLOOM;
	if(ImGui::Checkbox("Enable Bloom", &enableBloom))
	{
        flags_ = enableBloom ? flags_ | ENABLE_BLOOM : flags_ & ~ENABLE_BLOOM;
	}
    ImGui::SliderFloat("Exposure", &exposure_, 0.1f, 10.0f);
    ImGui::SliderInt("Blur Amount", &blurAmount_, 2, 20);
    ImGui::End();
}

void HelloBloomProgram::Render()
{
	if(cubeTexture_ == INVALID_TEXTURE_NAME)
	{
        cubeTexture_ = textureManager_.GetTextureName(cubeTextureId_);
        return;
	}
	if(flags_ & RESIZE_FRAMEBUFFER)
	{
        glDeleteFramebuffers(1, &hdrFbo_);
        glDeleteFramebuffers(2, &pingpongFbo_[0]);

        glDeleteTextures(2, &colorBuffers_[0]);
        glDeleteTextures(2, &pingpongColorBuffers_[0]);

        glDeleteRenderbuffers(1, &rbo_);
        CreateFramebuffer();
	}
	std::lock_guard<std::mutex> lock(updateMutex_);
    const auto view = camera_.GenerateViewMatrix();
    const auto projection = camera_.GenerateProjectionMatrix();
	//1. hdr pass
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFbo_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cubeShader_.Bind();
    cubeShader_.SetMat4("view", view);
    cubeShader_.SetMat4("projection", projection);
    cubeShader_.SetTexture("diffuseTexture", cubeTexture_, 0);

	for(size_t i = 0; i < lights_.size(); i++)
    {
        cubeShader_.SetVec3("lights["+std::to_string(i)+"}.Position", lights_[i].position_);
        cubeShader_.SetVec3("lights[" + std::to_string(i) + "].Color", lights_[i].color_);
    }
    cubeShader_.SetInt("lightNmb", lights_.size());
    cubeShader_.SetVec3("viewPos", camera_.position);
	for(const auto& transform : cubeTransforms_)
	{
        auto model = Mat4f::Identity;
        model = Transform3d::Translate(model, transform.position);
        model = Transform3d::Rotate(model, transform.angle, transform.axis);
        model = Transform3d::Scale(model, transform.scale);
        cubeShader_.SetMat4("model", model);
        cubeShader_.SetMat4("transposeInverseModel", model.Inverse().Transpose());

        cube_.Draw();
	}
    lightShader_.Bind();
    lightShader_.SetMat4("view", view);
    lightShader_.SetMat4("projection", projection);
	for(const auto& light : lights_)
	{
        auto model = Mat4f::Identity;
        model = Transform3d::Translate(model, light.position_);
        model = Transform3d::Scale(model, Vec3f(0.25f));
        lightShader_.SetMat4("model", model);
        lightShader_.SetVec3("lightColor", light.color_);
        cube_.Draw();
	}
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 2. blur bright fragments with two-pass Gaussian Blur 
    // --------------------------------------------------
    //
    bool horizontal = true, firstIteration = true;
    if (flags_ & ENABLE_BLOOM)
    {
        blurShader_.Bind();
        blurShader_.SetInt("image", 0);
        for (int i = 0; i < blurAmount_; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFbo_[horizontal]);
            blurShader_.SetInt("horizontal", horizontal);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, firstIteration ? colorBuffers_[1] : pingpongColorBuffers_[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
            screenPlane_.Draw();
            horizontal = !horizontal;
            if (firstIteration)
                firstIteration = false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    // 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
    // --------------------------------------------------------------------------------------------------------------------------
    bloomShader_.Bind();
    bloomShader_.SetTexture("scene", colorBuffers_[0], 0);
    bloomShader_.SetTexture("bloomBlur", pingpongColorBuffers_[!horizontal], 1);
    bloomShader_.SetInt("bloom", flags_ & ENABLE_BLOOM);
    bloomShader_.SetFloat("exposure", exposure_);
    screenPlane_.Draw();
}

void HelloBloomProgram::CreateFramebuffer()
{
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    // configure (floating point) framebuffers
    // ---------------------------------------

    glGenFramebuffers(1, &hdrFbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFbo_);
    // create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)

    glGenTextures(2, colorBuffers_);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers_[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, config.windowSize.x, config.windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        // attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers_[i], 0);
    }
    // create and attach depth buffer (renderbuffer)

    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, config.windowSize.x, config.windowSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    // finally check if framebuffer is complete
    glCheckFramebuffer();
    glCheckError();
    // ping-pong-framebuffer for blurring

    glGenFramebuffers(2, pingpongFbo_);
    glGenTextures(2, &pingpongColorBuffers_[0]);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFbo_[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorBuffers_[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, config.windowSize.x, config.windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorBuffers_[i], 0);
        glCheckFramebuffer();
        glCheckError();
    }
}

void HelloBloomProgram::OnEvent(const SDL_Event& event)
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

}