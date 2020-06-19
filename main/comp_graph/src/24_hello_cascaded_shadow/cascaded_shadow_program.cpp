#include "24_hello_cascaded_shadow/cascaded_shadow_program.h"
#include "imgui.h"
#include "mathematics/aabb.h"

namespace neko
{

void HelloCascadedShadowProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	glCheckError();
	plane_.Init();
	// Create the FBO
	glGenFramebuffers(1, &fbo_);

	// Create the depth buffer
	glGenTextures(shadowMaps_.size(), &shadowMaps_[0]);

	for (auto shadowMap : shadowMaps_)
	{
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMaps_[0], 0);

	// Disable writes to the color buffer
	GLenum drawBuffers = GL_NONE;
	glDrawBuffers(1, &drawBuffers);
	glReadBuffer(GL_NONE);

	CheckFramebuffer();
	glCheckError();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	simpleDepthShader_.LoadFromFile(
		config.dataRootPath + "shaders/24_hello_cascaded_shadow/simple_depth.vert",
		config.dataRootPath + "shaders/24_hello_cascaded_shadow/simple_depth.frag"
	);
	shadowShader_.LoadFromFile(
		config.dataRootPath + "shaders/24_hello_cascaded_shadow/shadow.vert",
		config.dataRootPath + "shaders/24_hello_cascaded_shadow/shadow.frag"
	);
	brickWall_.SetPath(config.dataRootPath + "sprites/brickwall/brickwall.jpg");
	brickWall_.LoadFromDisk();
	dragonModel_.LoadModel(config.dataRootPath + "model/dragon/dragon.obj");
	glGenTextures(1, &whiteTexture_);
	glBindTexture(GL_TEXTURE_2D, whiteTexture_);
	unsigned char white[] = { 255, 255, 255 };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glCheckError();

	camera_.position = Vec3f(0, 3.0f, -3.0f);
	camera_.LookAt(Vec3f::forward * camera_.farPlane / 2.0f);
	camera_.farPlane = 100.0f;
}

void HelloCascadedShadowProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
}

void HelloCascadedShadowProgram::Destroy()
{
	simpleDepthShader_.Destroy();
	shadowShader_.Destroy();
	glDeleteFramebuffers(1, &fbo_);
	glDeleteTextures(shadowMaps_.size(), &shadowMaps_[0]);
	dragonModel_.Destroy();
	plane_.Destroy();
}

void HelloCascadedShadowProgram::DrawImGui()
{
	ImGui::Begin("Cascaded Shadow Program");

	ImGui::SliderFloat("Near Ratio", &cascadedNearRatio_, 0.0001f, cascadedMiddleRatio_);
	ImGui::SliderFloat("Middle Ratio", &cascadedMiddleRatio_, cascadedNearRatio_, 1.0f);
	if (ImGui::InputFloat3("Light Direction", &lights_[0].direction[0]))
	{
		lights_[1].direction = lights_[2].direction = lights_[0].direction;
	}
	ImGui::Checkbox("Enable Cascade Color", &enableCascadeColor_);
	ImGui::End();
}

void HelloCascadedShadowProgram::Render()
{
	if (!dragonModel_.IsLoaded())
	{
		return;
	}
	if (!brickWall_.IsLoaded())
	{
		return;
	}
	std::lock_guard<std::mutex> lock(updateMutex_);
	glCheckError();
	//Cascade Shadow Pass
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	simpleDepthShader_.Bind();
	for(int i = 0; i < 3; i++)
	{
		ShadowPass(i);
	}
	//Render scene from camera
	const auto& config = BasicEngine::GetInstance()->config;
	glViewport(0, 0, config.windowSize.x, config.windowSize.y);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	shadowShader_.Bind();

	shadowShader_.SetMat4("view", camera_.GenerateViewMatrix());
	shadowShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
	shadowShader_.SetTexture("material.texture_diffuse1", whiteTexture_);
	for (size_t i = 0; i < lights_.size(); i++)
	{
		shadowShader_.SetMat4("lightSpaceMatrices[" + std::to_string(i) + "]", lights_[i].lightSpaceMatrix);
		shadowShader_.SetVec3("lights[" + std::to_string(i) + "].direction", lights_[i].direction);
	}
	shadowShader_.SetVec3("viewPos", camera_.position);
	for (size_t i = 0; i < shadowMaps_.size(); i++)
	{
		shadowShader_.SetTexture("lights[" + std::to_string(i) + "].shadowMap", shadowMaps_[i], i + 3);
	}
	shadowShader_.SetBool("enableCascadeColor", enableCascadeColor_);
	shadowShader_.SetFloat("farPlane", camera_.farPlane);
	shadowShader_.SetFloat("bias", shadowBias_);
	shadowShader_.SetFloat("maxNearCascade", camera_.farPlane * cascadedNearRatio_);
	shadowShader_.SetFloat("maxMiddleCascade", camera_.farPlane * cascadedMiddleRatio_);
	RenderScene(shadowShader_);
}

void HelloCascadedShadowProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}

Camera2D HelloCascadedShadowProgram::CalculateOrthoLight(float cascadeNear, float cascadeFar, Vec3f lightDir) const
{
	Camera2D lightCamera;
	lightCamera.position = Vec3f::zero;
	lightCamera.LookAt(lightDir);

	const auto lightView = lightCamera.GenerateViewMatrix();

	const auto view = camera_.GenerateViewMatrix();
	const auto viewInverse = view.Inverse();
	const auto tanHalfFovY = Tan(camera_.fovY / 2.0f);
	const auto tanHalfFovX = Tan(camera_.GetFovX() / 2.0f);

	const float nearX = cascadeNear * tanHalfFovX;
	const float nearY = cascadeNear * tanHalfFovY;
	const float farX = cascadeFar * tanHalfFovX;
	const float farY = cascadeFar * tanHalfFovY;

	Vec4f frustumCorners[8] =
	{
		// near face
		Vec4f(nearX,   nearY, cascadeNear, 1.0),
		Vec4f(-nearX,   nearY, cascadeNear, 1.0),
		Vec4f(nearX,   -nearY, cascadeNear, 1.0),
		Vec4f(-nearX,   -nearY, cascadeNear, 1.0),

		// far face
		Vec4f(farX, farY, cascadeFar, 1.0),
		Vec4f(-farX, farY, cascadeFar, 1.0),
		Vec4f(farX, -farY, cascadeFar, 1.0),
		Vec4f(-farX, -farY, cascadeFar, 1.0),
	};

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::min();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::min();

	for (const auto& corner : frustumCorners)
	{

		const auto vW = viewInverse * corner;
		const auto frustumCornersLight = lightView * vW;


		minX = std::min(minX, frustumCornersLight.x);
		maxX = std::max(maxX, frustumCornersLight.x);
		minY = std::min(minY, frustumCornersLight.y);
		maxY = std::max(maxY, frustumCornersLight.y);
		minZ = std::min(minZ, frustumCornersLight.z);
		maxZ = std::max(maxZ, frustumCornersLight.z);
	}
	
	lightCamera.right = (maxX-minX)/2.0f;
	lightCamera.left = -lightCamera.right;
	lightCamera.top = (maxY-minY)/2.0f;
	lightCamera.bottom = -lightCamera.top;
	lightCamera.farPlane = (maxZ-minZ);
	lightCamera.nearPlane = 0.0f;
	lightCamera.position += -lightCamera.rightDir*(maxX+minX)/2.0f - 
		lightCamera.upDir * (maxY + minY) / 2.0f - lightDir * minZ;
	return lightCamera;
}

void HelloCascadedShadowProgram::ShadowPass(int cascadeIndex)
{
	const auto cascadeNear = cascadeIndex == 0 ? camera_.nearPlane :
		cascadeIndex == 1 ? camera_.farPlane * cascadedNearRatio_ :
		camera_.farPlane * cascadedMiddleRatio_;
	const auto cascadeFar = cascadeIndex == 0 ? camera_.farPlane * cascadedNearRatio_ :
		cascadeIndex == 1 ? camera_.farPlane * cascadedMiddleRatio_ :
		camera_.farPlane;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMaps_[cascadeIndex], 0);
	glClear(GL_DEPTH_BUFFER_BIT);
	

	lightCamera_ = CalculateOrthoLight(cascadeNear, cascadeFar, lights_[cascadeIndex].direction);

	lights_[cascadeIndex].lightSpaceMatrix = 
		lightCamera_.GenerateProjectionMatrix() * lightCamera_.GenerateViewMatrix();
	
	simpleDepthShader_.SetFloat("lightFarPlane", lightCamera_.farPlane);
	simpleDepthShader_.SetMat4("lightSpaceMatrix",
		lights_[cascadeIndex].lightSpaceMatrix);

	RenderScene(simpleDepthShader_);
}

void HelloCascadedShadowProgram::RenderScene(const gl::Shader& shader)
{
	for (int z = 0; z < 5; z++)
	{
		for (int x = 0; x < 2; x++)
		{
			auto model = Mat4f::Identity;
			model = Transform3d::Scale(model, Vec3f::one * 0.2f);
			model = Transform3d::Translate(model, 
				Vec3f(-10.0f * float(x), 0.0f, 10.0f * float(z) + 5.0f));
			shader.SetMat4("model", model);
			shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
			dragonModel_.Draw(shader);
		}
	}
	auto model = Mat4f::Identity;
	model = Transform3d::Rotate(model, Quaternion::AngleAxis(degree_t(-90.0f), Vec3f::right));
	model = Transform3d::Scale(model, Vec3f::one * camera_.farPlane);
	model = Transform3d::Translate(model, Vec3f::forward * camera_.farPlane / 2.0f);
	shader.SetTexture("material.texture_diffuse1", brickWall_);
	shader.SetMat4("model", model);
	shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
	plane_.Draw();
}
}
