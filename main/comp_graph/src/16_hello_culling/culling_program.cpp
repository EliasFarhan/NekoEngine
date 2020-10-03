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

#include "16_hello_culling/culling_program.h"
#include "imgui.h"

namespace neko
{
void HelloCullingProgram::Init()
{
	textureManager_.Init();
	const auto& config = BasicEngine::GetInstance()->config;

	model_.LoadModel(config.dataRootPath + "model/nanosuit2/nanosuit.obj");
    modelShader_.LoadFromFile(
            config.dataRootPath + "shaders/16_hello_culling/model.vert",
            config.dataRootPath + "shaders/16_hello_culling/model.frag");
	camera_.position = Vec3f(0, 3, 3);
	camera_.WorldLookAt(Vec3f::zero);
	cube_.Init();
	cubeTextureId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/container.jpg");

}

void HelloCullingProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);	textureManager_.Update(dt);
}

void HelloCullingProgram::Destroy()
{
	model_.Destroy();
	modelShader_.Destroy();
	cube_.Destroy();
	textureManager_.Destroy();
	
}

void HelloCullingProgram::DrawImGui()
{
	ImGui::Begin("Culling Program");
	bool isCulling = flags_ & CULLING;
	if(ImGui::Checkbox("Culling", &isCulling))
	{
		flags_ = isCulling ? flags_ | CULLING : flags_ & ~CULLING;
	}
	bool isBack = flags_ & BACK_CULLING;
	if(ImGui::Checkbox("Back", &isBack))
	{
		flags_ = isBack ? flags_ | BACK_CULLING : flags_ & ~BACK_CULLING;
	}
	bool isCcw = flags_ & CCW;
	if(ImGui::Checkbox("CCW", &isCcw))
	{
		flags_ = isCcw ? flags_ | CCW : flags_ & ~CCW;
	}
	ImGui::End();
}

void HelloCullingProgram::Render()
{
	if (!model_.IsLoaded())
	{
		return;
	}
	if (cubeTexture_ == INVALID_TEXTURE_NAME)
	{
		cubeTexture_ = textureManager_.GetTexture(cubeTextureId_).name;
		return;
	}
	std::lock_guard<std::mutex> lock(updateMutex_);
	if (flags_ & CULLING)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(flags_ & BACK_CULLING ? GL_BACK : GL_FRONT);
		glFrontFace(flags_ & CCW ? GL_CCW : GL_CW);
	}
	modelShader_.Bind();
	modelShader_.SetMat4("view", camera_.GenerateViewMatrix());
	modelShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
	auto model = Mat4f::Identity;
	model = Transform3d::Scale(model, Vec3f::one*0.1f);
	modelShader_.SetMat4("model", model);
	modelShader_.SetMat4("transposeInverseModel", model.Inverse().Transpose());
	model_.Draw(modelShader_);
	model = Mat4f::Identity;
	model = Transform3d::Translate(model, Vec3f::left * 2.0f);
	modelShader_.SetMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTexture_);
	cube_.Draw();
	
	if(flags_ & CULLING)
	{
		glDisable(GL_CULL_FACE);
	}
}

void HelloCullingProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
