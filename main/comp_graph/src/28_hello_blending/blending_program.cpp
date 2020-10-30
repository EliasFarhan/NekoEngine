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

#include "28_hello_blending/blending_program.h"
#include "imgui.h"

namespace neko
{
void HelloBlendingProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	cutoffShader_.LoadFromFile(
		config.dataRootPath + "shaders/28_hello_blending/blending.vert",
		config.dataRootPath + "shaders/28_hello_blending/blending.frag");
	camera_.Init();
	windowTextureId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/blending_transparent_window.png");
	
	cubeTextureId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/container.jpg");
	
	plane_.Init();
	cube_.Init();

	glGenTextures(1, &whiteTexture_);
	glBindTexture(GL_TEXTURE_2D, whiteTexture_);
	unsigned char white[] = { 255, 255, 255 };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glCheckError();
	textureManager_.Init();
}

void HelloBlendingProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
	textureManager_.Update(dt);
}

void HelloBlendingProgram::Destroy()
{
	cutoffShader_.Destroy();
	textureManager_.Destroy();
	plane_.Destroy();
	cube_.Destroy();
}

void HelloBlendingProgram::DrawImGui()
{
	ImGui::Begin("Blending Program");
	bool enableBlending = flags_ & ENABLE_BLENDING;
	if(ImGui::Checkbox("Enable Blending", &enableBlending))
	{
		flags_ = enableBlending ? flags_ | ENABLE_BLENDING : flags_ & ~ENABLE_BLENDING;
	}
	bool enableSorting = flags_ & ENABLE_SORTING;
	if(ImGui::Checkbox("Enable Sorting", &enableSorting))
	{
		flags_ = enableSorting ? flags_ | ENABLE_SORTING : flags_ & ~ENABLE_SORTING;
	}
	ImGui::End();
}

void HelloBlendingProgram::Render()
{
	if (windowTexture_ == INVALID_TEXTURE_NAME )
	{
		windowTexture_ = textureManager_.GetTexture(windowTextureId_).name;
		if (windowTexture_ == INVALID_TEXTURE_NAME)
			return;
	}
	if ( cubeTexture_ == INVALID_TEXTURE_NAME)
	{
		cubeTexture_ = textureManager_.GetTexture(cubeTextureId_).name;
		if (cubeTexture_ == INVALID_TEXTURE_NAME)
			return;
	}
	std::lock_guard<std::mutex> lock(updateMutex_);
	if(flags_ & ENABLE_BLENDING)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	cutoffShader_.Bind();
	cutoffShader_.SetMat4("view", camera_.GenerateViewMatrix());
	cutoffShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
	//Draw floor

	cutoffShader_.SetTexture("texture1", whiteTexture_, 0);
	auto model = Mat4f::Identity;
	model = Transform3d::Rotate(model, Quaternion::AngleAxis(degree_t(-90.0f), Vec3f::right));
	model = Transform3d::Scale(model, Vec3f::one * 5.0f);
	model = Transform3d::Translate(model, Vec3f::down * 0.5f);
	cutoffShader_.SetMat4("model", model);
	plane_.Draw();

	//Draw cube
	cutoffShader_.SetTexture("texture1", cubeTexture_, 0);

	model = Mat4f::Identity;
	model = Transform3d::Translate(model, Vec3f(-1.0f, 0.0f, -1.0f));
	cutoffShader_.SetMat4("model", model);

	cube_.Draw();
	model = Mat4f::Identity;
	model = Transform3d::Translate(model, Vec3f(2.0f, 0.0f, 0.0f));
	cutoffShader_.SetMat4("model", model);
	cube_.Draw();

	
	//Draw windows
	cutoffShader_.SetTexture("texture1", windowTexture_, 0);
	const std::array<Vec3f, 5> windowPositions
	{
		Vec3f(-1.5f, 0.0f, -0.48f),
		Vec3f(1.5f, 0.0f, 0.51f),
		Vec3f(0.0f, 0.0f, 0.7f),
		Vec3f(-0.3f, 0.0f, -2.3f),
		Vec3f(0.5f, 0.0f, -0.6f)
	};
	if(flags_ & ENABLE_SORTING)
	{
		std::array<Vec3f, 5> sortedWindows = windowPositions;
		std::sort(sortedWindows.begin(), sortedWindows.end(),
			[this](const Vec3f& v1, const Vec3f& v2)
			{
				return (v1 - camera_.position).SquareMagnitude() > (v2 - camera_.position).SquareMagnitude();
			});
		for (const auto& position : sortedWindows)
		{
			model = Mat4f::Identity;
			model = Transform3d::Translate(model, position);
			cutoffShader_.SetMat4("model", model);
			plane_.Draw();
		}
	}
	else
	{
		for (const auto& position : windowPositions)
		{
			model = Mat4f::Identity;
			model = Transform3d::Translate(model, position);
			cutoffShader_.SetMat4("model", model);
			plane_.Draw();
		}
	}
	if(flags_ & ENABLE_BLENDING)
	{
		glDisable(GL_BLEND);
	}

}

void HelloBlendingProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
