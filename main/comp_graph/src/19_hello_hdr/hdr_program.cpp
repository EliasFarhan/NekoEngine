#include "19_hello_hdr/hdr_program.h"
#include "imgui.h"

namespace neko
{


void HelloHdrProgram::Init()
{
    const auto& config = BasicEngine::GetInstance()->config;
    cube_.Init();
    cubeShader_.LoadFromFile(
            config.dataRootPath + "shaders/19_hello_hdr/tunnel.vert",
            config.dataRootPath + "shaders/19_hello_hdr/tunnel.frag");
    cubeTexture_.SetTextureFlags(gl::Texture::TextureFlags(gl::Texture::MIRROR_REPEAT_WRAP | gl::Texture::DEFAULT));
    cubeTexture_.SetPath(config.dataRootPath+"sprites/brickwall/brickwall.jpg");
    cubeTexture_.LoadFromDisk();

    glGenFramebuffers(1, &hdrFbo_);
    // create floating point color buffer
    glGenTextures(1, &hdrColorBuffer_);
    glBindTexture(GL_TEXTURE_2D, hdrColorBuffer_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, config.windowSize.x, config.windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
    hdrPlane_.Init();
    hdrShader_.LoadFromFile(
        config.dataRootPath + "shaders/19_hello_hdr/hdr_screen.vert",
        config.dataRootPath + "shaders/19_hello_hdr/hdr_screen.frag");
    camera_.position = Vec3f::zero;
    camera_.LookAt(camera_.position+Vec3f::forward);

}

void HelloHdrProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    const auto& config = BasicEngine::GetInstance()->config;
    camera_.SetAspect(config.windowSize.x, config.windowSize.y);
    camera_.Update(dt);
}

void HelloHdrProgram::Destroy()
{
    cubeTexture_.Destroy();
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
    if (!cubeTexture_.IsLoaded())
    {
        return;
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
    glBindTexture(GL_TEXTURE_2D, cubeTexture_.GetTextureId());
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
    hdrShader_.SetInt("hdrBuffer", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrColorBuffer_);
    hdrShader_.SetBool("hdr", flags_&ENABLE_HDR);
    hdrShader_.SetFloat("exposure", exposure_);
    hdrPlane_.Draw();

}

void HelloHdrProgram::OnEvent(const SDL_Event& event)
{
    camera_.OnEvent(event);
}
}