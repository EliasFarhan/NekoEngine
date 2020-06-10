#include "21_hello_shadow/shadow_program.h"


namespace neko
{


void HelloShadowProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	cube_.Init();
	floor_.Init();

	glGenFramebuffers(1, &depthMapFbo_);
	glGenTextures(1, &depthMap_);
	glBindTexture(GL_TEXTURE_2D, depthMap_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap_, 0);
	glDrawBuffers(1, GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		logDebug("[Error] Shadow depth map framebuffer is incomplete");
	}
	depthCamera_.SetSize(Vec2f::one * 10.0f);
}

void HelloShadowProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
}

void HelloShadowProgram::Destroy()
{
	cube_.Destroy();
	floor_.Destroy();
}

void HelloShadowProgram::DrawImGui()
{

}

void HelloShadowProgram::Render()
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo_);
	glClear(GL_DEPTH_BUFFER_BIT);
	simpleDepthShader_.Bind();
	simpleDepthShader_.SetMat4("view", depthCamera_.GenerateViewMatrix());
	simpleDepthShader_.SetMat4("projection", depthCamera_.GenerateProjectionMatrix());
	
	RenderScene(simpleDepthShader_);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, config.windowSize.x, config.windowSize.y);
	modelShader_.Bind();
}

void HelloShadowProgram::RenderScene(const gl::Shader& shader)
{
	
}


void HelloShadowProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}