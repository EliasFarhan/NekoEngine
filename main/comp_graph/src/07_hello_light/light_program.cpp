#include "07_hello_light/light_program.h"
#include "imgui.h"

namespace neko
{
void HelloLightProgram::Init()
{
	camera_.Init();
	cube_.Init();
	const auto config =  BasicEngine::GetInstance()->config;
	lightShader_.LoadFromFile(
		config.dataRootPath + "shaders/07_hello_light/lamp.vert",
		config.dataRootPath + "shaders/07_hello_light/lamp.frag");
	phongShader_.LoadFromFile(
		config.dataRootPath + "shaders/07_hello_light/light.vert",
		config.dataRootPath + "shaders/07_hello_light/light.frag");
}

void HelloLightProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
    time_ += dt.count();
	lightPos_ = Vec3f(
		Cos(radian_t(time_)),
		0.0f,
		Sin(radian_t(time_)))* lightDist_;
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);;
	camera_.Update(dt);
}

void HelloLightProgram::Destroy()
{
	cube_.Destroy();
	lightShader_.Destroy();
	phongShader_.Destroy();
}

void HelloLightProgram::DrawImGui()
{
	ImGui::Begin("Light program");
	ImGui::InputFloat("ambientStrength", &ambientStrength_);
	ImGui::InputFloat("diffuseStrength", &diffuseStrength_);
	ImGui::InputFloat("specularStrength", &specularStrength_);
	ImGui::InputInt("specularPow", &specularPow_);
	ImGui::ColorEdit3("Object Color", &objectColor_[0]);
	ImGui::ColorEdit3("Light Color", &lightColor_[0]);
	ImGui::End();
}

void HelloLightProgram::Render()
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const Mat4f view = camera_.GenerateViewMatrix();
	const Mat4f projection = camera_.GenerateProjectionMatrix();
    //Render cube light
    lightShader_.Bind();
	Mat4f model = Mat4f::Identity;
	model = Transform3d::Scale(model, Vec3f(0.2f, 0.2f, 0.2f));
	model = Transform3d::Translate(model, lightPos_);
	lightShader_.SetMat4("model", model);
	lightShader_.SetMat4("view", view);
	lightShader_.SetMat4("projection", projection);
   
	lightShader_.SetVec3("lightColor", lightColor_);
	cube_.Draw();
	
	//Render center cube
	phongShader_.Bind();
	model = Mat4f::Identity;
	phongShader_.SetMat4("model", model);
	phongShader_.SetMat4("view", view);
	phongShader_.SetMat4("projection", projection);
	phongShader_.SetVec3("lightPos", lightPos_);
	phongShader_.SetVec3("viewPos", camera_.position);
	phongShader_.SetVec3("objectColor", objectColor_);
	phongShader_.SetVec3("lightColor", lightColor_);
	phongShader_.SetFloat("ambientStrength", ambientStrength_);
	phongShader_.SetFloat("diffuseStrength", diffuseStrength_);
	phongShader_.SetFloat("specularStrength", specularStrength_);
	phongShader_.SetInt("specularPow", specularPow_);
	
	const auto inverseTransposeModel = model.Inverse().Transpose();
	phongShader_.SetMat4("inverseTransposeModel", inverseTransposeModel);
	cube_.Draw();
	
}

void HelloLightProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
