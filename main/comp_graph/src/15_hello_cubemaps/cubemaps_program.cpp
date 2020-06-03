#include "15_hello_cubemaps/cubemaps_program.h"

namespace neko
{
void HelloCubemapsProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	cube_.Init();
	skyboxTexture_ = gl::LoadCubemap({
        config.dataRootPath+"sprites/skybox/right.jpg",
		config.dataRootPath+"sprites/skybox/left.jpg",
		config.dataRootPath+"sprites/skybox/top.jpg",
		config.dataRootPath+"sprites/skybox/bottom.jpg",
		config.dataRootPath+"sprites/skybox/front.jpg",
		config.dataRootPath+"sprites/skybox/back.jpg"
	});
	skyboxShader_.LoadFromFile(
		config.dataRootPath + "shaders/15_hello_cubemaps/skybox.vert",
		config.dataRootPath + "shaders/15_hello_cubemaps/skybox.frag"
	);
	camera_.position = Vec3f(0, 3, 3);
	camera_.LookAt(Vec3f::zero);
}

void HelloCubemapsProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
}

void HelloCubemapsProgram::Destroy()
{
	glDeleteTextures(1, &skyboxTexture_);
	cube_.Destroy();
	skyboxShader_.Destroy();
}

void HelloCubemapsProgram::DrawImGui()
{
}

void HelloCubemapsProgram::Render()
{
	std::lock_guard<std::mutex> lock(updateMutex_);

	const auto view = camera_.GenerateViewMatrix();
	const auto projection = camera_.GenerateProjectionMatrix();
	
	glDepthMask(GL_FALSE);
	skyboxShader_.Bind();
	skyboxShader_.SetMat4("view", Mat4f(view.ToMat3()));
	skyboxShader_.SetMat4("projection", projection);
	skyboxShader_.SetInt("skybox", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture_);
	cube_.Draw();
	glDepthMask(GL_TRUE);
}

void HelloCubemapsProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
