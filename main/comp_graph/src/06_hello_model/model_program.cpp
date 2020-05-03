#include "06_hello_model/model_program.h"
#include "mathematics/vector.h"
#include "mathematics/matrix.h"
#include "mathematics/transform.h"

namespace neko
{
void HelloModelProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;

	const std::string path = config.dataRootPath + "data/model/nanosuit2/nanosuit.obj";
	model_.LoadModel(path);
	camera_.Init();
	shader_.LoadFromFile(
		config.dataRootPath + "data/shaders/06_hello_model/model.vert",
		config.dataRootPath + "data/shaders/06_hello_model/model.frag");



}
void HelloModelProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	camera_.Update(dt);
	const auto& config = BasicEngine::GetInstance()->config;

	projection_ = Transform3d::Perspective(
		degree_t(45.0f),
		static_cast<float>(config.windowSize.x) / config.windowSize.y,
		0.1f,
		100.0f);
}

void HelloModelProgram::Destroy()
{
	model_.Destroy();
	shader_.Destroy();
}

void HelloModelProgram::DrawImGui()
{
}

void HelloModelProgram::Render()
{
	if (shader_.GetProgram() == 0)
		return;
	std::lock_guard<std::mutex> lock(updateMutex_);
	shader_.Bind();
	shader_.SetMat4("view", camera_.GenerateViewMatrix());
	shader_.SetMat4("projection", projection_);
	Mat4f model = Mat4f::Identity;
	model = Transform3d::Rotate(model, degree_t(180.0f), Vec3f::up);
	model = Transform3d::Scale(model, Vec3f(0.1f, 0.1f, 0.1f));
	shader_.SetMat4("model", model);
	model_.Draw(shader_);
}

void HelloModelProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
