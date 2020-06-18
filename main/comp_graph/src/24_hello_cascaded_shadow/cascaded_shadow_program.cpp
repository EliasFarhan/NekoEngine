#include "24_hello_cascaded_shadow/cascaded_shadow_program.h"
#include "imgui.h"
#include "mathematics/aabb.h"

namespace neko
{

	void HelloCascadedShadowProgram::Init()
	{
		const auto& config = BasicEngine::GetInstance()->config;
		glCheckError();
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
			config.dataRootPath+"shaders/24_hello_cascaded_shadow/simple_depth.vert",
			config.dataRootPath+"shaders/24_hello_cascaded_shadow/simple_depth.frag"
		);
		shadowShader_.LoadFromFile(
			config.dataRootPath + "shaders/24_hello_cascaded_shadow/shadow.vert",
			config.dataRootPath + "shaders/24_hello_cascaded_shadow/shadow.frag"
		);
		dragonModel_.LoadModel(config.dataRootPath + "model/dragon/dragon.obj");
		glGenTextures(1, &whiteTexture_);
		glBindTexture(GL_TEXTURE_2D, whiteTexture_);
		unsigned char white []= { 255, 255, 255 };
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glCheckError();
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
	}

	void HelloCascadedShadowProgram::DrawImGui()
	{
		ImGui::Begin("Cascaded Shadow Program");

		ImGui::SliderFloat("Near Ratio", &cascadedNearRatio_, 0.0001f, cascadedMiddleRatio_);
		ImGui::SliderFloat("Middle Ratio", &cascadedMiddleRatio_, cascadedNearRatio_, 1.0f);
		ImGui::InputFloat3("Light Direction", &light_.direction[0]);
		ImGui::End();
	}

	void HelloCascadedShadowProgram::Render()
	{
		if(!dragonModel_.IsLoaded())
		{
			return;
		}
		std::lock_guard<std::mutex> lock(updateMutex_);
		glCheckError();
		const auto dirY = camera_.up;
		const auto dirX = (light_.direction - Vec3f::Dot(light_.direction, dirY) * dirY).Normalized();
		const auto dirZ = Vec3f::Cross(dirX, dirY).Normalized();

		const auto lightDir = light_.direction.Normalized();
		const auto cameraDir = -camera_.reverseDirection;
		const auto fovX = camera_.GetFovX();
		const auto rightQuaternion = Quaternion::AngleAxis(fovX / 2.0f, camera_.up);
		const auto leftQuaternion = Quaternion::AngleAxis(-fovX / 2.0f, camera_.up);
		const auto cameraDirLeft = Vec3f(Transform3d::RotationMatrixFrom(rightQuaternion) * Vec4f(cameraDir));
		const auto cameraDirRight = Vec3f(Transform3d::RotationMatrixFrom(leftQuaternion) * Vec4f(cameraDir));
		
		const float deltaNearFar = camera_.farPlane - camera_.nearPlane;

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
		
		//Near BB
		simpleDepthShader_.Bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMaps_[0], 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		const Vec3f nearSides[4] =
		{
			camera_.position+cameraDirLeft*(camera_.farPlane*cascadedNearRatio_),
			camera_.position+cameraDirRight*(camera_.farPlane*cascadedNearRatio_),
			camera_.position+cameraDirLeft*(camera_.nearPlane),
			camera_.position+cameraDirRight*(camera_.nearPlane),
		};
		Vec2f nearX = Vec2f(std::numeric_limits<float>::infinity(), 0.0f);
		Vec2f nearZ = Vec2f(std::numeric_limits<float>::infinity(), 0.0f);
		for(auto side : nearSides)
		{
			nearX.x = std::min(nearX.x, Vec3f::Dot(dirX, side));
			nearX.y = std::max(nearX.y, Vec3f::Dot(dirX, side));
			nearZ.x = std::min(nearZ.x, Vec3f::Dot(dirZ, side));
			nearZ.y = std::max(nearZ.y, Vec3f::Dot(dirZ, side));
		}
		Aabb2d nearAabb;
		nearAabb.lowerLeftBound = Vec2f(nearX.x, nearZ.x);
		nearAabb.upperRightBound = Vec2f(nearX.y, nearZ.y);

		const auto nearPos = nearAabb.CalculateCenter();
		lightCamera_.position = dirX*nearPos.x+dirZ*nearPos.y;
		lightCamera_.SetSize(nearAabb.CalculateExtends());
		lightCamera_.LookAt(lightCamera_.position + lightDir);

		simpleDepthShader_.SetMat4("lightSpaceMatrix",
			lightCamera_.GenerateProjectionMatrix() * lightCamera_.GenerateViewMatrix());

		RenderScene(simpleDepthShader_);
		
		//Middle BB
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMaps_[1], 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		const Vec3f middleSides[4] =
		{
			camera_.position + cameraDirLeft * (camera_.farPlane * cascadedNearRatio_),
			camera_.position + cameraDirRight * (camera_.farPlane * cascadedNearRatio_),
			camera_.position + cameraDirLeft * (camera_.farPlane*cascadedMiddleRatio_),
			camera_.position + cameraDirRight * (camera_.farPlane*cascadedMiddleRatio_),
		};
		Vec2f middleX = Vec2f(std::numeric_limits<float>::infinity(), 0.0f);
		Vec2f middleZ = Vec2f(std::numeric_limits<float>::infinity(), 0.0f);
		for (auto side : middleSides)
		{
			middleX.x = std::min(middleX.x, Vec3f::Dot(dirX, side));
			middleX.y = std::max(middleX.y, Vec3f::Dot(dirX, side));
			middleZ.x = std::min(middleZ.x, Vec3f::Dot(dirZ, side));
			middleZ.y = std::max(middleZ.y, Vec3f::Dot(dirZ, side));
		}
		Aabb2d middleAabb;
		middleAabb.lowerLeftBound = Vec2f(middleX.x, middleZ.x);
		middleAabb.upperRightBound = Vec2f(middleX.y, middleZ.y);

		const auto middlePos = middleAabb.CalculateCenter();
		lightCamera_.position = dirX * middlePos.x + dirZ * middlePos.y;
		lightCamera_.SetSize(middleAabb.CalculateExtends());
		lightCamera_.LookAt(lightCamera_.position + lightDir);

		simpleDepthShader_.SetMat4("lightSpaceMatrix",
			lightCamera_.GenerateProjectionMatrix() * lightCamera_.GenerateViewMatrix());

		RenderScene(simpleDepthShader_);
		
		//Far BB
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMaps_[1], 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		const Vec3f farSides[4] =
		{
			camera_.position + cameraDirLeft * (camera_.farPlane),
			camera_.position + cameraDirRight * (camera_.farPlane),
			camera_.position + cameraDirLeft * (camera_.farPlane * cascadedMiddleRatio_),
			camera_.position + cameraDirRight * (camera_.farPlane * cascadedMiddleRatio_),
		};
		Vec2f farX = Vec2f(std::numeric_limits<float>::infinity(), 0.0f);
		Vec2f farZ = Vec2f(std::numeric_limits<float>::infinity(), 0.0f);
		for (auto side : farSides)
		{
			farX.x = std::min(farX.x, Vec3f::Dot(dirX, side));
			farX.y = std::max(farX.y, Vec3f::Dot(dirX, side));
			farZ.x = std::min(farZ.x, Vec3f::Dot(dirZ, side));
			farZ.y = std::max(farZ.y, Vec3f::Dot(dirZ, side));
		}
		Aabb2d farAabb;
		farAabb.lowerLeftBound = Vec2f(farX.x, farZ.x);
		farAabb.upperRightBound = Vec2f(farX.y, farZ.y);

		const auto farPos = farAabb.CalculateCenter();
		lightCamera_.position = dirX * farPos.x + dirZ * farPos.y;
		lightCamera_.SetSize(farAabb.CalculateExtends());
		lightCamera_.LookAt(lightCamera_.position + lightDir);

		simpleDepthShader_.SetMat4("lightSpaceMatrix",
			lightCamera_.GenerateProjectionMatrix() * lightCamera_.GenerateViewMatrix());

		RenderScene(simpleDepthShader_);

		//Render scene from camera
		const auto& config = BasicEngine::GetInstance()->config;
		glViewport(0, 0, config.windowSize.x, config.windowSize.y);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		shadowShader_.Bind();

		shadowShader_.SetMat4("view", camera_.GenerateViewMatrix());
		shadowShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
		shadowShader_.SetTexture("material.texture_diffuse1", whiteTexture_);
		
		RenderScene(shadowShader_);
	}

	void HelloCascadedShadowProgram::OnEvent(const SDL_Event& event)
	{
		camera_.OnEvent(event);
	}

	void HelloCascadedShadowProgram::RenderScene(const gl::Shader& shader)
	{
		
	}
}
