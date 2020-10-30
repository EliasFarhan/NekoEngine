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

#include "12_hello_stencil/stencil_program.h"
#include "imgui.h"

namespace neko
{
void HelloStencilProgam::Init()
{
	textureManager_.Init();
	cube_.Init();
	plane_.Init();

	const auto& config = BasicEngine::GetInstance()->config;
    cubeShader_.LoadFromFile(config.dataRootPath + "shaders/12_hello_stencil/cube.vert",
                             config.dataRootPath + "shaders/12_hello_stencil/cube.frag");
    floorShader_.LoadFromFile(config.dataRootPath + "shaders/12_hello_stencil/floor.vert",
                              config.dataRootPath + "shaders/12_hello_stencil/floor.frag");
	cubeTextureId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/container.jpg");


	camera_.position = Vec3f(0.0f, 4.0f, 4.0f);
	camera_.WorldLookAt(Vec3f::zero);
}

void HelloStencilProgam::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);	textureManager_.Update(dt);
}

void HelloStencilProgam::Destroy()
{
	cube_.Destroy();
	textureManager_.Destroy();
	cubeShader_.Destroy();

	floorShader_.Destroy();
	plane_.Destroy();
	
}

void HelloStencilProgam::DrawImGui()
{
	ImGui::Begin("Stencil Program");
	bool useStencil = flags_ & USE_STENCIL;
	if(ImGui::Checkbox("Use Stencil", &useStencil))
	{
		flags_ = useStencil ? flags_ | USE_STENCIL : flags_ & ~USE_STENCIL;
	}
	bool removeOnlyDepth = flags_ & REMOVE_ONLY_DEPTH;
	if(ImGui::Checkbox("Remove Only Depth", &removeOnlyDepth))
	{
		flags_ = removeOnlyDepth ? flags_ | REMOVE_ONLY_DEPTH : flags_ & ~REMOVE_ONLY_DEPTH;
	}
	ImGui::End();
}

void HelloStencilProgam::Render()
{
	if (cubeTexture_ == INVALID_TEXTURE_NAME)
	{
		cubeTexture_ = textureManager_.GetTexture(cubeTextureId_).name;
		return;
	}
	std::lock_guard<std::mutex> lock(updateMutex_);
	//Draw upper cube
	glEnable(GL_DEPTH_TEST);
	const auto view = camera_.GenerateViewMatrix();
	const auto projection = camera_.GenerateProjectionMatrix();

	auto model = Mat4f::Identity;
	model = Transform3d::Translate(model, Vec3f(0, 1, 0));
	
	
	cubeShader_.Bind();
	cubeShader_.SetMat4("view", view);
	cubeShader_.SetMat4("projection", projection);
	cubeShader_.SetMat4("model", model);
	cubeShader_.SetVec3("overrideColor", Vec3f::one);

	cubeShader_.SetInt("ourTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTexture_);

	cube_.Draw();
	if(flags_ & USE_STENCIL)
	{
		// Draw floor
		glEnable(GL_STENCIL_TEST);

		glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //only put stencil value if depth pass
		glStencilMask(0xFF); // Write to stencil buffer

		glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
	}
	if(flags_ & REMOVE_ONLY_DEPTH)
	{
		glDepthMask(GL_FALSE); // Don't write to depth buffer, but still checking depth
	}
	floorShader_.Bind();
	floorShader_.SetMat4("view", view);
	floorShader_.SetMat4("projection", projection);
	model = Mat4f::Identity;
	model = Transform3d::Rotate(model, degree_t(90.0f), Vec3f::right);
	floorShader_.SetMat4("model", model);
	floorShader_.SetVec3("color", Vec3f::one * 0.1f);
	plane_.Draw();
	//Draw cube reflection
	if(flags_ & USE_STENCIL)
	{
		glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
		glStencilMask(0x00); // Don't write anything to stencil buffer
	}
	if(flags_ & REMOVE_ONLY_DEPTH)
	{
		glDepthMask(GL_TRUE); // Write to depth buffer
	}
	cubeShader_.Bind();
	model = Mat4f::Identity;
	model = Transform3d::Scale(model, Vec3f(1, -1, 1));
	model = Transform3d::Translate(model, Vec3f(0, -1, 0));
	cubeShader_.SetMat4("model", model);
	cubeShader_.SetVec3("overrideColor", Vec3f::one * 0.3f);
	cube_.Draw();
	if(flags_ & USE_STENCIL)
	{
		glDisable(GL_STENCIL_TEST);
	}

}

void HelloStencilProgam::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
