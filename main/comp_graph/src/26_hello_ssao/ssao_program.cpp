#include "26_hello_ssao/ssao_program.h"

namespace neko
{
void HelloSsaoProgram::Init()
{
    const auto& config = BasicEngine::GetInstance()->config;
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
}

void HelloSsaoProgram::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(updateMutex_);
    const auto& config = BasicEngine::GetInstance()->config;
    camera_.SetAspect(config.windowSize.x, config.windowSize.y);
    camera_.Update(dt);
}

void HelloSsaoProgram::Destroy()
{
    DestroyFramebuffer();
}

void HelloSsaoProgram::DrawImGui()
{
}

void HelloSsaoProgram::Render()
{
    std::lock_guard<std::mutex> lock(updateMutex_);
}

void HelloSsaoProgram::OnEvent(const SDL_Event& event)
{
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

    CheckFramebuffer();
    glCheckError();
	
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // also create framebuffer to hold SSAO processing stage 
	// -----------------------------------------------------

    glGenFramebuffers(1, &ssaoFbo_);  glGenFramebuffers(1, &ssaoBlurFbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFbo_);

    // SSAO color buffer
    glGenTextures(1, &ssaoColorBuffer_);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, config.windowSize.x, config.windowSize.y, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer_, 0);
    CheckFramebuffer();
    // and blur stage
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFbo_);
    glGenTextures(1, &ssaoColorBufferBlur_);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, config.windowSize.x, config.windowSize.y, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur_, 0);
    CheckFramebuffer();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
}
}
