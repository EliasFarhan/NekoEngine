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

#include "22_hello_blinn/blinn_program.h"
#include "imgui.h"

namespace neko
{
void HelloBlinnProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
    modelShader_.LoadFromFile(config.dataRootPath + "shaders/22_hello_blinn/model.vert",
                              config.dataRootPath + "shaders/22_hello_blinn/model.frag");
    blinnShader_.LoadFromFile(config.dataRootPath + "shaders/22_hello_blinn/model.vert",
                              config.dataRootPath + "shaders/22_hello_blinn/model_blinn.frag");
	floor_.Init();
	floorTextureId_ = textureManager_.LoadTexture(
		config.dataRootPath + "sprites/brickwall/brickwall.jpg", 
		Texture::TextureFlags(Texture::REPEAT_WRAP | Texture::DEFAULT));

	model_.LoadModel(config.dataRootPath + "model/nanosuit2/nanosuit.obj");

	camera_.position = Vec3f(0.0f, 3.0f, 3.0f);
	camera_.WorldLookAt(Vec3f::zero);
	textureManager_.Init();
}

void HelloBlinnProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
	dt_ += dt.count();
	lightPos_ = Vec3f(3.0f*Cos(radian_t(dt_)), 3.0f, 3.0f*Sin(radian_t(dt_)));

	textureManager_.Update(dt);
}

void HelloBlinnProgram::Destroy()
{
	modelShader_.Destroy();
	blinnShader_.Destroy();
	floor_.Destroy();
	textureManager_.Destroy();
	model_.Destroy();
}

void HelloBlinnProgram::DrawImGui()
{
	ImGui::Begin("Blinn Program");
	bool enableBlinn = flags_ & ENABLE_BLINN;
	if(ImGui::Checkbox("Enable Blinn", &enableBlinn))
	{
		flags_ = enableBlinn ? flags_ | ENABLE_BLINN : flags_ & ~ENABLE_BLINN;
	}
	ImGui::SliderFloat("Floor Texture Resolution", &floorResolution_, 0.5f, 12.0f);
	ImGui::SliderInt("Specular Pow", &specularPow_, 1, 256);
	bool onlySpecular = flags_ & ONLY_SPECULAR;
	if(ImGui::Checkbox("Only Specular", &onlySpecular))
	{
		flags_ = onlySpecular ? flags_ | ONLY_SPECULAR : flags_ & ~ONLY_SPECULAR;
	}
	ImGui::End();
}

void HelloBlinnProgram::Render()
{
	if(!model_.IsLoaded())
		return;
	if (floorTexture_ == INVALID_TEXTURE_NAME)
	{
		floorTexture_ = textureManager_.GetTexture(floorTextureId_).name;
		return;
	}
	std::lock_guard<std::mutex> lock(updateMutex_);

	auto& shader = flags_ & ENABLE_BLINN ? blinnShader_ : modelShader_;
	shader.Bind();
	shader.SetFloat("light.constant", 1.0f);
	shader.SetFloat("light.linear", 0.09f);
	shader.SetFloat("light.quadratic", 0.032f);
	shader.SetMat4("view", camera_.GenerateViewMatrix());
	shader.SetMat4("projection", camera_.GenerateProjectionMatrix());
	auto model = Mat4f::Identity;
	model = Transform3d::Scale(model, Vec3f(0.1f, 0.1f, 0.1f));
	shader.SetMat4("model", model);
	shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
	shader.SetVec3("light.position", lightPos_);
	shader.SetVec3("viewPos", camera_.position);
	shader.SetFloat("material.resolution", 1.0f);
	shader.SetFloat("material.shininess", 32.0f);
	shader.SetBool("onlySpecular", flags_ & ONLY_SPECULAR);
	model_.Draw(shader);

	model = Mat4f::Identity;
	model = Transform3d::Rotate(model, degree_t(-90.0f), Vec3f::right);
	shader.SetMat4("model", model);
	shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
	shader.SetInt("material.texture_diffuse1", 0);
	shader.SetFloat("material.resolution", floorResolution_);

	shader.SetFloat("material.shininess", static_cast<float>(specularPow_));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorTexture_);
	floor_.Draw();
}

void HelloBlinnProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
