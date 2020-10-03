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
#include "23_hello_point_shadow/point_shadow_program.h"
#include "imgui.h"

namespace neko
{


void HelloPointShadowProgram::Init()
{

	textureManager_.Init();
	const auto& config = BasicEngine::GetInstance()->config;
	cube_.Init();

	simpleDepthShader_.LoadFromFile(
		config.dataRootPath + "shaders/23_hello_point_shadow/simpleDepth.vert",
		config.dataRootPath + "shaders/23_hello_point_shadow/simpleDepth.frag");
	cubeShader_.LoadFromFile(config.dataRootPath + "shaders/23_hello_point_shadow/shadow.vert",
		config.dataRootPath + "shaders/23_hello_point_shadow/shadow.frag");
	lightCubeShader_.LoadFromFile(config.dataRootPath + "shaders/23_hello_point_shadow/lamp.vert",
		config.dataRootPath + "shaders/23_hello_point_shadow/lamp.frag");

	cubeTextureId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/brickwall/brickwall.jpg");

	glGenFramebuffers(1, &depthMapFbo_);
	// create depth cubemap texture
	glGenTextures(1, &depthCubemap_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap_);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, 
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo_);
	GLenum drawBuffers[] = { GL_NONE };
	glDrawBuffers(1, drawBuffers);
	glReadBuffer(GL_NONE);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, depthCubemap_, 0);

	glCheckFramebuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	camera3D_.position = Vec3f::one * 3.0f;
	camera3D_.WorldLookAt(Vec3f::zero);

	lightCamera_.position = Vec3f::zero;
	lightCamera_.farPlane = 50.0f;
	lightCamera_.nearPlane = 0.1f;
	lightCamera_.fovY = degree_t(90.0f);
	lightCamera_.aspect = 1.0f;

}

void HelloPointShadowProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera3D_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera3D_.Update(dt);
	dt_ += dt.count();
	lightCamera_.position = 4.0f * Vec3f(Sin(radian_t(dt_)), 0.0f, Sin(radian_t(dt_)));
	textureManager_.Update(dt);
}

void HelloPointShadowProgram::Destroy()
{
	glDeleteFramebuffers(1, &depthMapFbo_);
	glDeleteTextures(1, &depthCubemap_);

	cube_.Destroy();
	lightCubeShader_.Destroy();
	cubeShader_.Destroy();
	simpleDepthShader_.Destroy();
	textureManager_.Destroy();
}

void HelloPointShadowProgram::DrawImGui()
{
    ImGui::Begin("Point Shadow Program");
    ImGui::InputFloat3("Light Pos", &lightCamera_.position[0]);
    ImGui::InputFloat("Shadow Bias", &bias_);
    ImGui::End();
}

void HelloPointShadowProgram::Render()
{
	if (cubeTexture_ == INVALID_TEXTURE_NAME)
	{
		cubeTexture_ = textureManager_.GetTexture(cubeTextureId_).name;
		return;
	}
	std::lock_guard<std::mutex> lock(updateMutex_);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo_);

	const Vec3f lightDirs[6] =
	{
		Vec3f::right,
		Vec3f::left,
		Vec3f::up,
		Vec3f::down,
		Vec3f::forward,
		Vec3f::back
	};
	const Vec3f lightUps[6] =
	{
		Vec3f::up,
		Vec3f::up,
		Vec3f::back,
		Vec3f::forward,
		Vec3f::up,
		Vec3f::up
	};
	simpleDepthShader_.Bind();
	simpleDepthShader_.SetVec3("lightPos", lightCamera_.position);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	for (int i = 0; i < 6; i++)
	{
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			depthCubemap_, 0);
		glClear(GL_DEPTH_BUFFER_BIT);

		lightCamera_.WorldLookAt(lightCamera_.position+lightDirs[i], lightUps[i]);
		const auto lightSpaceMatrix = lightCamera_.GenerateProjectionMatrix() * lightCamera_.GenerateViewMatrix();
		simpleDepthShader_.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		simpleDepthShader_.SetFloat("lightFarPlane", lightCamera_.farPlane);
		simpleDepthShader_.SetVec3("lightDir", lightDirs[i]);
		RenderScene(simpleDepthShader_);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	const auto& config = BasicEngine::GetInstance()->config;
	glViewport(0, 0, config.windowSize.x, config.windowSize.y);
	cubeShader_.Bind();
	const auto view = camera3D_.GenerateViewMatrix();
	const auto projection = camera3D_.GenerateProjectionMatrix();
	cubeShader_.SetMat4("view", view);
	cubeShader_.SetMat4("projection", projection);
	cubeShader_.SetVec3("light.lightPos", lightCamera_.position);
	cubeShader_.SetVec3("viewPos", camera3D_.position);

	cubeShader_.SetFloat("lightFarPlane", lightCamera_.farPlane);
	cubeShader_.SetFloat("bias", bias_);
	//Render the scene with shadow
	cubeShader_.SetTexture("material.texture_diffuse1", cubeTexture_, 0);
	cubeShader_.SetInt("shadowMap", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap_);
	RenderScene(cubeShader_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	//Render the white light cube
	lightCubeShader_.Bind();
	lightCubeShader_.SetVec3("lightColor", Vec3f::one);
	lightCubeShader_.SetMat4("view", view);
	lightCubeShader_.SetMat4("projection", projection);
	auto model = Mat4f::Identity;
	model = Transform3d::Translate(model, lightCamera_.position);
	model = Transform3d::Scale(model, Vec3f::one * 0.2f);
	lightCubeShader_.SetMat4("model", model);
	cube_.Draw();
}

void HelloPointShadowProgram::OnEvent(const SDL_Event& event)
{
	camera3D_.OnEvent(event);
}

void HelloPointShadowProgram::RenderScene(const gl::Shader& shader)
{
	auto model = Mat4f::Identity;
	shader.SetBool("inverseNormals", false);

	for (const auto& transform : transforms_)
	{
		model = Mat4f::Identity;
		model = Transform3d::Translate(model, transform.position);
		model = Transform3d::Rotate(model, transform.angle, transform.axis);
		model = Transform3d::Scale(model, transform.scale);
		shader.SetMat4("model", model);
		shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
		cube_.Draw();
	}

	//Render bigger cube
	model = Mat4f::Identity;
	model = Transform3d::Scale(model, Vec3f::one * 10.0f);
	shader.SetMat4("model", model);
	shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
	shader.SetBool("inverseNormals", true);
	cube_.Draw();
}
}