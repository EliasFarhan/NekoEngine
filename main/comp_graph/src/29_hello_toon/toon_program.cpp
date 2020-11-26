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

#include "29_hello_toon/toon_program.h"
#include "imgui.h"

namespace neko
{
void HelloToonShadingProgram::Init()
{

	const auto& config = BasicEngine::GetInstance()->GetConfig();
	const std::string path = config.dataRootPath + "model/nanosuit2/nanosuit.obj";
	glCheckError();
	modelId_ = modelManager_.LoadModel(path);
	camera_.Init();
	toonShader_.LoadFromFile(
		config.dataRootPath + "shaders/29_hello_toon/toon.vert",
		config.dataRootPath + "shaders/29_hello_toon/toon.frag");

	glCheckError();
}

void HelloToonShadingProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->GetConfig();
	textureManager_.Update(dt);
	modelManager_.Update(dt);
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
	dt_ += dt.count();
	light_.position = 4.0f * Vec3f(Cos(radian_t(dt_)), 0.5f, Sin(radian_t(dt_)));
}

void HelloToonShadingProgram::Destroy()
{
	textureManager_.Destroy();
	modelManager_.Destroy();
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
	if(!modelManager_.IsLoaded(modelId_))
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

	auto* mod = modelManager_.GetModel(modelId_);
	mod->Draw(shader);
}

void HelloToonShadingProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
