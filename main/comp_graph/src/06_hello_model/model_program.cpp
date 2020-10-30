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

#include "06_hello_model/model_program.h"
#include "mathematics/vector.h"
#include "mathematics/matrix.h"
#include "mathematics/transform.h"

namespace neko
{
void HelloModelProgram::Init()
{
	textureManager_.Init();
	const auto& config = BasicEngine::GetInstance()->config;

	const std::string path = config.dataRootPath + "model/nanosuit2/nanosuit.obj";
    glCheckError();
	model_.LoadModel(path);
	camera_.Init();
    shader_.LoadFromFile(
            config.dataRootPath + "shaders/06_hello_model/model.vert",
            config.dataRootPath + "shaders/06_hello_model/model.frag");

	glCheckError();


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
		100.0f);	textureManager_.Update(dt);
}

void HelloModelProgram::Destroy()
{
	model_.Destroy();
	shader_.Destroy();
	textureManager_.Destroy();
}

void HelloModelProgram::DrawImGui()
{
}

void HelloModelProgram::Render()
{
	if (shader_.GetProgram() == 0)
		return;
    glCheckError();
	std::lock_guard<std::mutex> lock(updateMutex_);
	if(!model_.IsLoaded())
		return;

	glCheckError();
	shader_.Bind();
	shader_.SetMat4("view", camera_.GenerateViewMatrix());
	shader_.SetMat4("projection", projection_);
	Mat4f model = Mat4f::Identity;
	model = Transform3d::Rotate(model, degree_t(180.0f), Vec3f::up);
	model = Transform3d::Scale(model, Vec3f(0.1f, 0.1f, 0.1f));
	shader_.SetMat4("model", model);
	shader_.SetMat4("normalMatrix", model.Inverse().Transpose());
	model_.Draw(shader_);
}

void HelloModelProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
