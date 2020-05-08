#include "07_hello_light/light_program.h"

namespace neko
{
void HelloLightProgram::Init()
{
	camera_.Init();
}

void HelloLightProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
    time_ += dt.count();
	const auto& config = BasicEngine::GetInstance()->config;
	projection_ = Transform3d::Perspective(
		degree_t(45.0f),
		static_cast<float>(config.windowSize.x) / config.windowSize.y,
		0.1f,
		100.0f);
	camera_.Update(dt);
}

void HelloLightProgram::Destroy()
{
}

void HelloLightProgram::DrawImGui()
{
}

void HelloLightProgram::Render()
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const Mat4f view = camera_.GenerateViewMatrix();

    //Light position
    lightShader_.Bind();

    Mat4f model = Mat4f::Identity;
    model = Transform3d::Translate(model, Vec3f(Cos(radian_t(time_)), 0.0f, Sin(radian_t(time_))));
}

void HelloLightProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
