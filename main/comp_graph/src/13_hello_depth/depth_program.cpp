#include "13_hello_depth/depth_program.h"
#include "imgui.h"

namespace neko
{
void HelloDepthProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	cube_.Init();
	floor_.Init();
	screenPlane_.Init();

	glGenFramebuffers(1, &fbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	
	glGenTextures(1, &screenTexture_);
	glBindTexture(GL_TEXTURE_2D, screenTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, config.windowSize.x, config.windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture_, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenTextures(1, &depthTexture_);
	glBindTexture(GL_TEXTURE_2D, depthTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, config.windowSize.x, config.windowSize.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture_, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{

		switch (status)
		{
		case GL_FRAMEBUFFER_UNDEFINED:
			logDebug("[Error] Framebuffer is undefined!");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			logDebug("[Error] Framebuffer is unsupported!");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			logDebug("[Error] Framebuffer has incomplete attachment!");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			logDebug("[Error] Framebuffer has incomplete missing attachment!");
			break;
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	sceneShader_.LoadFromFile(config.dataRootPath + "shaders/99_hello_scene/cube.vert",
		config.dataRootPath + "shaders/99_hello_scene/cube.frag");
	screenShader_.LoadFromFile(config.dataRootPath + "shaders/13_hello_depth/screen.vert",
		config.dataRootPath + "shaders/13_hello_depth/screen.frag");
	depthOnlyShader_.LoadFromFile(config.dataRootPath + "shaders/13_hello_depth/screen.vert",
		config.dataRootPath + "shaders/13_hello_depth/screen_depth.frag");
	camera_.position = Vec3f(0, 4, 4);
	camera_.LookAt(Vec3f::zero);
}

void HelloDepthProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	camera_.Update(dt);
}

void HelloDepthProgram::Destroy()
{
	sceneShader_.Destroy();
	screenShader_.Destroy();
	depthOnlyShader_.Destroy();

	cube_.Destroy();
	screenPlane_.Destroy();
	floor_.Destroy();


	glDeleteFramebuffers(1, &fbo_);
	glDeleteTextures(1, &screenTexture_);
	glDeleteTextures(1, &depthTexture_);

}

void HelloDepthProgram::DrawImGui()
{
	ImGui::Begin("Depth Program");
	bool depthOnly = flags_ & DEPTH_ONLY;
	if (ImGui::Checkbox("Depth Only", &depthOnly))
	{
		flags_ = depthOnly ? flags_ | DEPTH_ONLY : flags_ & ~DEPTH_ONLY;
	}
	bool linearDepth = flags_ & LINEAR_DEPTH;
	if(ImGui::Checkbox("Linear Depth", &linearDepth))
	{
		flags_ = linearDepth ? flags_ | LINEAR_DEPTH : flags_ & ~LINEAR_DEPTH;
	}
	ImGui::End();
}

void HelloDepthProgram::Render()
{
	std::lock_guard<std::mutex> lock(updateMutex_);

	//Bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//Draw scene
	DrawScene();
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (flags_ & DEPTH_ONLY)
	{
		depthOnlyShader_.Bind();
		depthOnlyShader_.SetFloat("near", camera_.nearPlane);
		depthOnlyShader_.SetFloat("far", camera_.farPlane);
		depthOnlyShader_.SetInt("depthTexture", 0);
		depthOnlyShader_.SetBool("linearDepth", flags_ & LINEAR_DEPTH);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthTexture_);
		screenPlane_.Draw();
	}
	else
	{
		screenShader_.Bind();
		screenShader_.SetInt("screenTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenTexture_);
		screenPlane_.Draw();
	}
	glEnable(GL_DEPTH_TEST);
}

void HelloDepthProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}

void HelloDepthProgram::DrawScene()
{
	auto model = Mat4f::Identity;
	model = Transform3d::Translate(model, Vec3f(0, 0.5f, 0));

	sceneShader_.Bind();
	sceneShader_.SetMat4("model", model);
	sceneShader_.SetMat4("view", camera_.GenerateViewMatrix());
	sceneShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());

	sceneShader_.SetVec3("color", Vec3f(0.5f, 0.0f, 0.5f));
	cube_.Draw();

	model = Mat4f::Identity;
	model = Transform3d::Rotate(model, degree_t(90), Vec3f::right);
	model = Transform3d::Scale(model, Vec3f(5.0f, 5.0f, 5.0f));
	sceneShader_.SetMat4("model", model);
	sceneShader_.SetVec3("color", Vec3f(0.3f, 0.0f, 0.3f));
	floor_.Draw();

}
}
