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

#include "07_hello_light/light_program.h"
#include "imgui.h"

namespace neko
{
void HelloLightProgram::Init()
{
	camera_.Init();
	cube_.Init();
	const auto config =  BasicEngine::GetInstance()->GetConfig();
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
	const auto& config = BasicEngine::GetInstance()->GetConfig();
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
