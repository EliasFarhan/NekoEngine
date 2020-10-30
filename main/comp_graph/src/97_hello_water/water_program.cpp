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

#include "97_hello_water/water_program.h"

namespace neko
{
void HelloWaterProgram::Init()
{
	textureManager_.Init();
	const auto& config = BasicEngine::GetInstance()->config;
	skyboxCube_.Init();
	skyboxTexture_ = gl::LoadCubemap({
		config.dataRootPath + "sprites/skybox/right.jpg",
		config.dataRootPath + "sprites/skybox/left.jpg",
		config.dataRootPath + "sprites/skybox/top.jpg",
		config.dataRootPath + "sprites/skybox/bottom.jpg",
		config.dataRootPath + "sprites/skybox/front.jpg",
		config.dataRootPath + "sprites/skybox/back.jpg"
		});

	quad_.Init();
	skyboxShader_.LoadFromFile(
		config.dataRootPath + "shaders/15_hello_cubemaps/skybox.vert",
		config.dataRootPath + "shaders/15_hello_cubemaps/skybox.frag");
	model_.LoadModel(config.dataRootPath + "model/nanosuit2/nanosuit.obj");
	modelShader_.LoadFromFile(
		config.dataRootPath + "shaders/97_hello_water/model.vert", 
		config.dataRootPath + "shaders/97_hello_water/model.frag");
	waterShader_.LoadFromFile(
		config.dataRootPath + "shaders/97_hello_water/water.vert",
		config.dataRootPath + "shaders/97_hello_water/water.frag");

	dudvTexturerId_ = textureManager_.LoadTexture(
		config.dataRootPath + "sprites/water/waveDUDV.png");
	normalMapId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/water/waveNM.png");
	
	CreateFramebuffer();
	CreateDepthbuffer();
	camera_.Init();
}

void HelloWaterProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
	textureManager_.Update(dt);
	dt_ += dt.count();
}

void HelloWaterProgram::Destroy()
{
	skyboxCube_.Destroy();
	glDeleteTextures(1, &skyboxTexture_);
	quad_.Destroy();
	skyboxShader_.Destroy();
	modelShader_.Destroy();
	waterShader_.Destroy();
	textureManager_.Destroy();
	//Destroy framebuffers
	glDeleteFramebuffers(1, &reflectionFramebuffer_);
	glDeleteFramebuffers(1, &refractionFramebuffer_);
	glDeleteTextures(1, &reflectionColorBuffer_);
	glDeleteTextures(1, &refractionColorBuffer_);
	glDeleteRenderbuffers(1, &reflectionDepthBuffer_);
	glDeleteRenderbuffers(1, &refractionDepthBuffer_);

	glDeleteFramebuffers(1, &depthFramebuffer_);
	glDeleteTextures(1, &depthBuffer_);
}

void HelloWaterProgram::DrawImGui()
{
}

void HelloWaterProgram::Render()
{
	if (!model_.IsLoaded())
	{
		return;
	}
	if (dudvTexturer_ == INVALID_TEXTURE_NAME)
	{
		dudvTexturer_ = textureManager_.GetTexture(dudvTexturerId_).name;
		if (dudvTexturer_ == INVALID_TEXTURE_NAME)
			return;
	}
	if (normalMap_ == INVALID_TEXTURE_NAME)
	{
		normalMap_ = textureManager_.GetTexture(normalMapId_).name;
		if (normalMap_ == INVALID_TEXTURE_NAME)
			return;
	}
	std::lock_guard<std::mutex> lock(updateMutex_);

	glCheckError();
	if(resizeScreen_)
	{
		glDeleteFramebuffers(1, &depthFramebuffer_);
		glDeleteTextures(1, &depthBuffer_);
		CreateDepthbuffer();
		resizeScreen_ = false;
	}
	const auto view = camera_.GenerateViewMatrix();
	const auto projection = camera_.GenerateProjectionMatrix();
	std::function<void(int, const Mat4f&, const Mat4f&)> renderScene = [this](int passType, Mat4f view, Mat4f projection)
	{
		modelShader_.Bind();
		modelShader_.SetMat4("view", view);
		modelShader_.SetMat4("projection", projection);
		auto model = Mat4f::Identity;

		model = Transform3d::Rotate(model, degree_t(180.0f), Vec3f::up);
		model = Transform3d::Scale(model, Vec3f(0.1f));

		modelShader_.SetMat4("model", model);
		modelShader_.SetFloat("waterHeight", waterHeight_);
		modelShader_.SetInt("passType", passType);
		model_.Draw(modelShader_);
		//Render skybox
		glDepthFunc(GL_LEQUAL);
		skyboxShader_.Bind();
		skyboxShader_.SetMat4("view", Mat4f(view.ToMat3()));
		skyboxShader_.SetMat4("projection", projection);
		skyboxShader_.SetInt("skybox", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture_);
		skyboxCube_.Draw();
		glDepthFunc(GL_LESS);
		glCheckError();
	};
	//Reflection
	Camera3D underWaterCamera = static_cast<Camera3D>(camera_);
	underWaterCamera.reverseDir = Vec3f::Reflect(underWaterCamera.reverseDir, Vec3f::up);
	underWaterCamera.position.y = underWaterCamera.position.y - 2.0f * std::abs(underWaterCamera.position.y - waterHeight_);
	underWaterCamera.WorldLookAt(underWaterCamera.position - underWaterCamera.reverseDir, Vec3f::up);
	glViewport(0, 0, reflectionFrameSize.x, reflectionFrameSize.y);
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFramebuffer_);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene(1, underWaterCamera.GenerateViewMatrix(), underWaterCamera.GenerateProjectionMatrix());
	//Refraction
	glViewport(0, 0, refractionFrameSize.x, refractionFrameSize.y);
	glBindFramebuffer(GL_FRAMEBUFFER, refractionFramebuffer_);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene(2, view, projection);
	//Render Scene
	const auto& config = BasicEngine::GetInstance()->config;
	glViewport(0, 0, config.windowSize.x, config.windowSize.y);
	//Depth pass
	glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer_);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene(0, view, projection);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene(0, view, projection);
	//Draw water
	waterShader_.Bind();
	auto model = Mat4f::Identity;
	model = Transform3d::Rotate(model, degree_t(-90.0f), Vec3f::right);
	model = Transform3d::Scale(model, Vec3f(5.0f));
	model = Transform3d::Translate(model, Vec3f(0.0f,waterHeight_, 0.0f));
	waterShader_.SetMat4("model", model);
	waterShader_.SetMat4("view", view);
	waterShader_.SetMat4("projection", projection);
	waterShader_.SetFloat("timeSinceStart", dt_);
	waterShader_.SetTexture("reflectionMap", reflectionColorBuffer_, 0);
	waterShader_.SetTexture("refractionMap", refractionColorBuffer_, 1);
	waterShader_.SetTexture("dudvMap", dudvTexturer_, 2);
	waterShader_.SetTexture("normalMap", normalMap_, 3);
	waterShader_.SetTexture("depthMap", depthBuffer_, 4);
	waterShader_.SetFloat("waveStrength", 0.02f);
	waterShader_.SetVec3("viewPos", camera_.position);
	waterShader_.SetFloat("waveSpeed", 0.01f);
	quad_.Draw();
}

void HelloWaterProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
	if (event.type == SDL_WINDOWEVENT)
	{
		if (event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			resizeScreen_ = true;
		}
	}
}

void HelloWaterProgram::CreateFramebuffer()
{
	glCheckError();
	glGenFramebuffers(1, &reflectionFramebuffer_);
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFramebuffer_);

	glGenTextures(1, &reflectionColorBuffer_);
	glBindTexture(GL_TEXTURE_2D, reflectionColorBuffer_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, reflectionFrameSize.x, reflectionFrameSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionColorBuffer_, 0);

	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, attachments);

	glGenRenderbuffers(1, &reflectionDepthBuffer_);
	glBindRenderbuffer(GL_RENDERBUFFER, reflectionDepthBuffer_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, reflectionFrameSize.x, reflectionFrameSize.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, reflectionDepthBuffer_);

	glCheckFramebuffer();

	glGenFramebuffers(1, &refractionFramebuffer_);
	glBindFramebuffer(GL_FRAMEBUFFER, refractionFramebuffer_);

	glGenTextures(1, &refractionColorBuffer_);
	glBindTexture(GL_TEXTURE_2D, refractionColorBuffer_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, refractionFrameSize.x, refractionFrameSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractionColorBuffer_, 0);

	glDrawBuffers(1, attachments);

	glGenRenderbuffers(1, &refractionDepthBuffer_);
	glBindRenderbuffer(GL_RENDERBUFFER, refractionDepthBuffer_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, refractionFrameSize.x, refractionFrameSize.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, refractionDepthBuffer_);

	glCheckFramebuffer();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCheckError();

	
}

void HelloWaterProgram::CreateDepthbuffer()
{
	const auto& config = BasicEngine::GetInstance()->config;
	glGenFramebuffers(1, &depthFramebuffer_);
	glGenTextures(1, &depthBuffer_);
	glBindTexture(GL_TEXTURE_2D, depthBuffer_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
		config.windowSize.x,config.windowSize.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,
		GL_LEQUAL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer_, 0);
	GLenum drawBuffers = GL_NONE;
	glDrawBuffers(1, &drawBuffers);
	glReadBuffer(GL_NONE);

	glCheckFramebuffer();
	glCheckError();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}
