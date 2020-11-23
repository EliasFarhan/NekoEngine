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

#include "32_hello_ibl/ibl_program.h"
#include "imgui.h"
#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

namespace neko
{
void HelloIblProgram::Init()
{
	textureManager_.Init();
	const auto& config = BasicEngine::GetInstance()->GetConfig();
	sphere_.Init();
	quad_.Init();
	skybox_.Init();
	equiToCubemap_.LoadFromFile(
		config.dataRootPath + "shaders/32_hello_ibl/cube.vert",
		config.dataRootPath + "shaders/32_hello_ibl/cube.frag");

	skyboxShader_.LoadFromFile(
		config.dataRootPath + "shaders/32_hello_ibl/skybox.vert",
		config.dataRootPath + "shaders/32_hello_ibl/skybox.frag");
	pbrShader_.LoadFromFile(
		config.dataRootPath + "shaders/32_hello_ibl/pbr.vert",
		config.dataRootPath + "shaders/32_hello_ibl/pbr.frag");
	irradianceShader_.LoadFromFile(
		config.dataRootPath + "shaders/32_hello_ibl/cube.vert",
		config.dataRootPath + "shaders/32_hello_ibl/irradiance.frag");
	prefilterShader_.LoadFromFile(
		config.dataRootPath + "shaders/32_hello_ibl/cube.vert",
		config.dataRootPath + "shaders/32_hello_ibl/prefilter.frag");
	brdfShader_.LoadFromFile(
		config.dataRootPath + "shaders/32_hello_ibl/brdf.vert",
		config.dataRootPath + "shaders/32_hello_ibl/brdf.frag");
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

	hdrTextureId_ = textureManager_.LoadTexture(
		config.dataRootPath + "textures/Ridgecrest_Road_Ref.hdr", Texture::TextureFlags(
		Texture::TextureFlags::CLAMP_WRAP |
		Texture::TextureFlags::SMOOTH_TEXTURE |
		Texture::TextureFlags::FLIP_Y |
		Texture::TextureFlags::HDR));

	flags_ = FIRST_FRAME;

	glGenFramebuffers(1, &captureFbo_);
	glGenRenderbuffers(1, &captureRbo_);

	glCheckError();
}

void HelloIblProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->GetConfig();
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);	textureManager_.Update(dt);
}

void HelloIblProgram::Destroy()
{
	sphere_.Destroy();
	quad_.Destroy();
	skybox_.Destroy();

	textureManager_.Destroy();
	
	equiToCubemap_.Destroy();
	irradianceShader_.Destroy();
	prefilterShader_.Destroy();
	skyboxShader_.Destroy();
	pbrShader_.Destroy();
	brdfShader_.Destroy();

	glDeleteFramebuffers(1, &captureFbo_);
	glDeleteRenderbuffers(1, &captureRbo_);

	glDeleteTextures(1, &envCubemap_);
	glDeleteTextures(1, &irradianceMap_);
	glDeleteTextures(1, &prefilterMap_);
	glDeleteTextures(1, &brdfLUTTexture_);
}

void HelloIblProgram::DrawImGui()
{
	ImGui::Begin("Pbr Program");
	ImGui::ColorPicker3("Base Color", &baseColor_[0]);
	bool showIrradianceMap = flags_ & SHOW_IRRADIANCE;
	if(ImGui::Checkbox("Show Irradiance Map", &showIrradianceMap))
	{
		flags_ = showIrradianceMap ? flags_ | SHOW_IRRADIANCE : flags_ & ~SHOW_IRRADIANCE;
	}
	bool showPrefilterMap = flags_ & SHOW_PREFILTER;
	if(ImGui::Checkbox("Show Prefilter Map", &showPrefilterMap))
	{
		flags_ = showPrefilterMap ? flags_ | SHOW_PREFILTER : flags_ & ~SHOW_PREFILTER;
	}
	bool enableIrradiance = flags_ & ENABLE_IRRADIANCE;
	if(ImGui::Checkbox("Enable Irradiance", &enableIrradiance))
	{
		flags_ = enableIrradiance ? flags_ | ENABLE_IRRADIANCE : flags_ & ~ENABLE_IRRADIANCE;
	}
	bool enableSchlickRoughness = flags_ & ENABLE_SCHLICK_ROUGHNESS;
	if(ImGui::Checkbox("Enable Schlick Roughness", &enableSchlickRoughness))
	{
		flags_ = enableSchlickRoughness ? flags_ | ENABLE_SCHLICK_ROUGHNESS : flags_ & ~ENABLE_SCHLICK_ROUGHNESS;
	}
	bool enableIblSpecular = flags_ & ENABLE_IBL_SPECULAR;
	if(ImGui::Checkbox("Enable Specular IBL", &enableIblSpecular))
	{
		flags_ = enableIblSpecular ? flags_ | ENABLE_IBL_SPECULAR : flags_ & ~ENABLE_IBL_SPECULAR;
	}
	
	ImGui::End();
}

void HelloIblProgram::Render()
{

	std::lock_guard<std::mutex> lock(updateMutex_);
	if (hdrTexture_ == INVALID_TEXTURE_NAME)
	{
		hdrTexture_ = textureManager_.GetTextureName(hdrTextureId_);
		if (hdrTexture_ == INVALID_TEXTURE_NAME)
			return;
	}
	if (flags_ & FIRST_FRAME)
	{
#ifdef EASY_PROFILE_USE
		EASY_BLOCK("Generate IBL textures");
#endif
		glDepthFunc(GL_LEQUAL);
		GenerateCubemap();
		GenerateDiffuseIrradiance();
		GeneratePrefilter();
		GenerateLUT();
		glDepthFunc(GL_LESS);
		const auto& config = BasicEngine::GetInstance()->GetConfig();
		glViewport(0, 0, config.windowSize.x, config.windowSize.y);
		flags_ = flags_ & ~FIRST_FRAME;
	}
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Render IBL");
#endif
	const auto view = camera_.GenerateViewMatrix();
	const auto projection = camera_.GenerateProjectionMatrix();

	//Render PBR spheres
	const int nrRows = 7;
	const int nrColumns = 7;
	pbrShader_.Bind();
	pbrShader_.SetBool("enableIrradiance", flags_ & ENABLE_IRRADIANCE);
	pbrShader_.SetBool("enableIblSpecular", flags_ & ENABLE_IBL_SPECULAR);
	pbrShader_.SetBool("enableSchlickRoughness", flags_ & ENABLE_SCHLICK_ROUGHNESS);
	pbrShader_.SetBool("gammaCorrect", true);
	pbrShader_.SetFloat("ao", 1.0f);
	pbrShader_.SetVec3("albedo", baseColor_);
	pbrShader_.SetMat4("view", view);
	pbrShader_.SetVec3("viewPos", camera_.position);
	pbrShader_.SetMat4("projection", projection);
	pbrShader_.SetCubemap("irradianceMap", irradianceMap_, 0);
	pbrShader_.SetCubemap("prefilterMap", prefilterMap_, 1);
	pbrShader_.SetTexture("brdfLUT", brdfLUTTexture_, 2);
	for (size_t i = 0; i < lights_.size(); i++)
	{
		pbrShader_.SetVec3("lights[" + std::to_string(i) + "].position", lights_[i].position);
		pbrShader_.SetVec3("lights[" + std::to_string(i) + "].color", lights_[i].color);

	}
	for (int row = 0; row < nrRows; ++row)
	{
		pbrShader_.SetFloat("metallic", static_cast<float>(row) / static_cast<float>(nrRows - 1));
		for (int col = 0; col < nrColumns; ++col)
		{
			// we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
			// on direct lighting.
			pbrShader_.SetFloat("roughness", Clamp(static_cast<float>(col) / static_cast<float>(nrColumns - 1), 0.05f, 1.0f));

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
	//Render skybox
	glDepthFunc(GL_LEQUAL);
	skyboxShader_.Bind();
	skyboxShader_.SetMat4("view", view);
	skyboxShader_.SetMat4("projection", projection);
	skyboxShader_.SetCubemap("environmentMap", 
		flags_ & SHOW_PREFILTER? prefilterMap_ : (flags_ & SHOW_IRRADIANCE ? irradianceMap_ : envCubemap_), 0);
	skybox_.Draw();
	glDepthFunc(GL_LESS);
	glCheckError();
}

void HelloIblProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}

void HelloIblProgram::GenerateCubemap()
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Generate Cubemap");
#endif
	logDebug("Generate Cubemap");
    glBindFramebuffer(GL_FRAMEBUFFER, captureFbo_);
    glGenTextures(1, &envCubemap_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap_);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 1024, 1024, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glCheckError();

	glBindRenderbuffer(GL_RENDERBUFFER, captureRbo_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1024,1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, envCubemap_, 0);
	glCheckError();

	Camera3D captureCamera;
	captureCamera.position = Vec3f::zero;
	captureCamera.aspect = 1.0f;
	captureCamera.fovY = degree_t(90.0f);
	captureCamera.nearPlane = 0.1f;
	captureCamera.farPlane = 10.0f;


	equiToCubemap_.Bind();
	equiToCubemap_.SetTexture("equirectangularMap", hdrTexture_, 0);
	equiToCubemap_.SetMat4("projection", captureCamera.GenerateProjectionMatrix());
	glViewport(0, 0, 1024,1024);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFbo_);
	for (unsigned int i = 0; i < 6; ++i)
	{
		captureCamera.WorldLookAt(viewDirs[i], upDirs[i]);
		equiToCubemap_.SetMat4("view", captureCamera.GenerateViewMatrix());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap_, 0);
        glCheckFramebuffer();
        glCheckError();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skybox_.Draw();
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HelloIblProgram::GenerateDiffuseIrradiance()
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Generate Diffuse Irradiance");
#endif
	logDebug("Generate DIffuse Irradiance");

    glBindFramebuffer(GL_FRAMEBUFFER, captureFbo_);
    glGenTextures(1, &irradianceMap_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap_);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glCheckError();
	glBindRenderbuffer(GL_RENDERBUFFER, captureRbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, irradianceMap_, 0);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 32, 32);
    glCheckError();

	Camera3D captureCamera;
	captureCamera.position = Vec3f::zero;
	captureCamera.aspect = 1.0f;
	captureCamera.fovY = degree_t(90.0f);
	captureCamera.nearPlane = 0.1f;
	captureCamera.farPlane = 10.0f;

	
	irradianceShader_.Bind();
	irradianceShader_.SetMat4("projection", captureCamera.GenerateProjectionMatrix());
	irradianceShader_.SetCubemap("environmentMap", envCubemap_, 0);

	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFbo_);
	for(int i = 0; i < 6; i++)
	{
		captureCamera.WorldLookAt(viewDirs[i], upDirs[i]);
		irradianceShader_.SetMat4("view", captureCamera.GenerateViewMatrix());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap_, 0);
		glCheckFramebuffer();
        glCheckError();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox_.Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCheckError();
	
}

void HelloIblProgram::GeneratePrefilter()
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Generate Prefilter Convolution Map");
#endif
	logDebug("Generate Prefilter Convolution Map");
	Camera3D captureCamera;
	captureCamera.position = Vec3f::zero;
	captureCamera.aspect = 1.0f;
	captureCamera.fovY = degree_t(90.0f);
	captureCamera.nearPlane = 0.1f;
	captureCamera.farPlane = 10.0f;

    glBindFramebuffer(GL_FRAMEBUFFER, captureFbo_);
	glGenTextures(1, &prefilterMap_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap_);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minifcation filter to mip_linear 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glCheckError();
	// generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glCheckError();
	prefilterShader_.Bind();
	prefilterShader_.SetCubemap("environmentMap",envCubemap_, 0);
	prefilterShader_.SetMat4("projection", captureCamera.GenerateProjectionMatrix());


    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                           prefilterMap_, 0);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRbo_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, mipWidth, mipHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		prefilterShader_.SetFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			captureCamera.WorldLookAt(viewDirs[i], upDirs[i]);
			prefilterShader_.SetMat4("view", captureCamera.GenerateViewMatrix());
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
				prefilterMap_, mip);
            glCheckFramebuffer();
            glCheckError();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			skybox_.Draw();
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCheckError();
}

void HelloIblProgram::GenerateLUT()
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Generate BRDF LUT");
#endif
	logDebug("Generate BRDF LUT");

	glGenTextures(1, &brdfLUTTexture_);

	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFbo_);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRbo_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture_, 0);

	glViewport(0, 0, 512, 512);
	brdfShader_.Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	quad_.Draw();
	glCheckFramebuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCheckError();
}
}
