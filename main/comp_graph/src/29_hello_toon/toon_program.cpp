#include "29_hello_toon/toon_program.h"
#include "imgui.h"

namespace neko
{
void HelloToonShadingProgram::Init()
{

	const auto& config = BasicEngine::GetInstance()->config;
	const std::string path = config.dataRootPath + "model/nanosuit2/nanosuit.obj";
	glCheckError();
	model_.LoadModel(path);
	camera_.Init();
	toonShader_.LoadFromFile(
		config.dataRootPath + "shaders/29_hello_toon/toon.vert",
		config.dataRootPath + "shaders/29_hello_toon/toon.frag");

	glCheckError();
}

void HelloToonShadingProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
	dt_ += dt.count();
	light_.position = 4.0f * Vec3f(Cos(radian_t(dt_)), 0.5f, Sin(radian_t(dt_)));
}

void HelloToonShadingProgram::Destroy()
{
	model_.Destroy();
	toonShader_.Destroy();
}

void HelloToonShadingProgram::DrawImGui()
{
	ImGui::Begin("Toon Program");
	ImGui::SliderFloat("Light Ambient", &light_.ambient, 0.0f, 0.5f);
	ImGui::SliderInt("Toon Layers", &toonLayers_, 1, 8);
	ImGui::SliderInt("Specular Layers", &specularLayers_, 1, 8);

	ImGui::End();
}

void HelloToonShadingProgram::Render()
{
	if(!model_.IsLoaded())
	{
		return;
	}
	std::lock_guard<std::mutex> lock(updateMutex_);

	toonShader_.Bind();
	gl::Shader& shader = toonShader_;
	shader.SetMat4("view", camera_.GenerateViewMatrix());
	shader.SetMat4("projection", camera_.GenerateProjectionMatrix());
	Mat4f model = Mat4f::Identity;
	model = Transform3d::Rotate(model, degree_t(180.0f), Vec3f::up);
	model = Transform3d::Scale(model, Vec3f(0.1f, 0.1f, 0.1f));
	shader.SetMat4("model", model);
	shader.SetMat4("normalMatrix", model.Inverse().Transpose());

	//Toon shader specifics
	shader.SetInt("toonLayers", toonLayers_);
	shader.SetVec3("viewPos", camera_.position);
	shader.SetVec3("light.position", light_.position);
	shader.SetFloat("material.ambient", light_.ambient);

	model_.Draw(shader);
}

void HelloToonShadingProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
