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

#include "30_hello_pbr/pbr_program.h"
#include "imgui.h"
#include "mathematics/basic.h"

namespace neko
{
void HelloPbrProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->GetConfig();
	sphere_.Init();
	pbrShader_.LoadFromFile(
		config.dataRootPath + "shaders/30_hello_pbr/pbr.vert",
		config.dataRootPath + "shaders/30_hello_pbr/pbr.frag");
	camera_.position = Vec3f::forward * 30.0f;
	camera_.WorldLookAt(Vec3f::zero);

	lights_ = {
		{
		{Vec3f(-10.0f,  10.0f, 10.0f), Vec3f(300.0f, 300.0f, 300.0f)},
		{Vec3f(10.0f,  10.0f, 10.0f),Vec3f(300.0f, 300.0f, 300.0f)},
		{Vec3f(-10.0f, -10.0f, 10.0f),Vec3f(300.0f, 300.0f, 300.0f)},
		{Vec3f(10.0f, -10.0f, 10.0f),Vec3f(300.0f, 300.0f, 300.0f)},
		}
	};
}

void HelloPbrProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->GetConfig();
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
}

void HelloPbrProgram::Destroy()
{
	sphere_.Destroy();
	pbrShader_.Destroy();
	
}

void HelloPbrProgram::DrawImGui()
{
	ImGui::Begin("Pbr Program");
	ImGui::ColorPicker3("Base Color", &baseColor_[0]);
	ImGui::Checkbox("Gamma Correct", &gammaCorrect_);
	ImGui::End();
}

void HelloPbrProgram::Render()
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const int nrRows = 7;
	const int nrColumns = 7;
	pbrShader_.Bind();
	pbrShader_.SetBool("gammaCorrect", gammaCorrect_);
	pbrShader_.SetFloat("ao", 1.0f);
	pbrShader_.SetVec3("albedo", baseColor_);
	pbrShader_.SetMat4("view", camera_.GenerateViewMatrix());
	pbrShader_.SetVec3("viewPos", camera_.position);
	pbrShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
	for(size_t i = 0; i < lights_.size(); i++)
	{
		pbrShader_.SetVec3("lights[" + std::to_string(i) + "].position", lights_[i].position);
		pbrShader_.SetVec3("lights[" + std::to_string(i) + "].color", lights_[i].color);

	}
	for (int row = 0; row < nrRows; ++row)
	{
		pbrShader_.SetFloat("metallic", static_cast<float>(row) / static_cast<float>(nrRows-1));
		for (int col = 0; col < nrColumns; ++col)
		{
			// we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
			// on direct lighting.
			pbrShader_.SetFloat("roughness", Clamp(static_cast<float>(col) / static_cast<float>(nrColumns-1), 0.05f, 1.0f));

			auto model = Mat4f::Identity;
			model = Transform3d::Translate(model, Vec3f(
				static_cast<float>(col - nrColumns / 2) * spacing_,
				static_cast<float>(row - nrRows / 2) * spacing_,
				0.0f
			));
			pbrShader_.SetMat4("model", model);
			pbrShader_.SetMat4("normalMatrix", model.Inverse().Transpose());
			sphere_.Draw();
		}
	}
}

void HelloPbrProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
