#include "32_hello_ibl/ibl_program.h"
#include "imgui.h"
#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

namespace neko
{
void HelloIblProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	sphere_.Init();
	cube_.Init();
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
		config.dataRootPath + "shaders/32_hello_ibl/irradiance.vert",
		config.dataRootPath + "shaders/32_hello_ibl/irradiance.frag");
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

	hdrTexture_.SetTextureFlags(Texture::TextureFlags(
		Texture::TextureFlags::CLAMP_WRAP |
		Texture::TextureFlags::SMOOTH_TEXTURE |
		Texture::TextureFlags::HDR));
	hdrTexture_.SetPath(config.dataRootPath + "textures/Ridgecrest_Road_Ref.hdr");
	hdrTexture_.LoadFromDisk();
	flags_ = FIRST_FRAME;

	glGenFramebuffers(1, &captureFbo_);
	glGenRenderbuffers(1, &captureRbo_);

	glCheckError();
}

void HelloIblProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
}

void HelloIblProgram::Destroy()
{
	sphere_.Destroy();
	cube_.Destroy();
	skybox_.Destroy();
	pbrShader_.Destroy();
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
	ImGui::End();
}

void HelloIblProgram::Render()
{

	std::lock_guard<std::mutex> lock(updateMutex_);
	if (!hdrTexture_.IsLoaded())
	{
		return;
	}
	if (flags_ & FIRST_FRAME)
	{
		glDepthFunc(GL_LEQUAL);
		GenerateCubemap();
		GenerateDiffuseIrradiance();

		glDepthFunc(GL_LESS);
		const auto& config = BasicEngine::GetInstance()->config;
		glViewport(0, 0, config.windowSize.x, config.windowSize.y);
		flags_ = flags_ & ~FIRST_FRAME;
	}

	const auto view = camera_.GenerateViewMatrix();
	const auto projection = camera_.GenerateProjectionMatrix();

	//Render PBR spheres
	const int nrRows = 7;
	const int nrColumns = 7;
	pbrShader_.Bind();
	pbrShader_.SetBool("gammaCorrect", true);
	pbrShader_.SetFloat("ao", 1.0f);
	pbrShader_.SetVec3("albedo", baseColor_);
	pbrShader_.SetMat4("view", view);
	pbrShader_.SetVec3("viewPos", camera_.position);
	pbrShader_.SetMat4("projection", projection);
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
	skyboxShader_.SetCubemap("environmentMap", flags_ & SHOW_IRRADIANCE ? irradianceMap_ : envCubemap_, 0);
	cube_.Draw();
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
	glBindRenderbuffer(GL_RENDERBUFFER, captureRbo_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 512, 512);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, captureRbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glGenTextures(1, &envCubemap_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap_);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Camera3D captureCamera;
	captureCamera.position = Vec3f::zero;
	captureCamera.aspect = 1.0f;
	captureCamera.fovY = degree_t(90.0f);
	captureCamera.nearPlane = 0.1f;
	captureCamera.farPlane = 10.0f;


	glBindFramebuffer(GL_FRAMEBUFFER, captureFbo_);
	equiToCubemap_.Bind();
	equiToCubemap_.SetTexture("equirectangularMap", hdrTexture_, 0);
	equiToCubemap_.SetMat4("projection", captureCamera.GenerateProjectionMatrix());
	glViewport(0, 0, 512, 512);
	for (unsigned int i = 0; i < 6; ++i)
	{
		captureCamera.WorldLookAt(viewDirs[i], upDirs[i]);
		equiToCubemap_.SetMat4("view", captureCamera.GenerateViewMatrix());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap_, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube_.Draw();
	}
	glCheckFramebuffer();
	glCheckError();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HelloIblProgram::GenerateDiffuseIrradiance()
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Generate Diffuse Irradiance");
#endif
	logDebug("Generate Irradiance Map");
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

	glBindFramebuffer(GL_FRAMEBUFFER, captureFbo_);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRbo_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 32, 32);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	Camera3D captureCamera;
	captureCamera.position = Vec3f::zero;
	captureCamera.aspect = 1.0f;
	captureCamera.fovY = degree_t(90.0f);
	captureCamera.nearPlane = 0.1f;
	captureCamera.farPlane = 10.0f;

	
	irradianceShader_.Bind();
	irradianceShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
	irradianceShader_.SetCubemap("environmentMap", envCubemap_, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFbo_);
	glViewport(0, 0, 32, 32);
	for(int i = 0; i < 6; i++)
	{
		captureCamera.WorldLookAt(viewDirs[i], upDirs[i]);
		irradianceShader_.SetMat4("view", captureCamera.GenerateViewMatrix());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap_, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cube_.Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}
}
