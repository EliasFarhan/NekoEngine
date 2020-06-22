#include "27_hello_cutoff/cutoff_program.h"
#include "imgui.h"

namespace neko
{
	void HelloCutoffProgram::Init()
	{
		const auto& config = BasicEngine::GetInstance()->config;
		cutoffShader_.LoadFromFile(
			config.dataRootPath + "shaders/27_hello_cutoff/cutoff.vert",
			config.dataRootPath + "shaders/27_hello_cutoff/cutoff.frag");
		camera_.Init();
		grassTexture_.SetPath(config.dataRootPath + "sprites/grass.png");
		grassTexture_.SetTextureFlags(gl::Texture::TextureFlags::CLAMP_WRAP);
		
		grassTexture_.LoadFromDisk();
		cubeTexture_.SetPath(config.dataRootPath + "sprites/container.jpg");
		cubeTexture_.LoadFromDisk();

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
	}

	void HelloCutoffProgram::Update(seconds dt)
	{
		std::lock_guard<std::mutex> lock(updateMutex_);
		const auto& config = BasicEngine::GetInstance()->config;
		camera_.SetAspect(config.windowSize.x, config.windowSize.y);
		camera_.Update(dt);
	}

	void HelloCutoffProgram::Destroy()
	{
		cutoffShader_.Destroy();
		grassTexture_.Destroy();
		cubeTexture_.Destroy();
		plane_.Destroy();
		cube_.Destroy();
	}

	void HelloCutoffProgram::DrawImGui()
	{
		ImGui::Begin("Cutoff Program");
		ImGui::Checkbox("Enable Cutoff", &enableCutoff_);
		ImGui::End();
	}

	void HelloCutoffProgram::Render()
	{
		if(!grassTexture_.IsLoaded() || !cubeTexture_.IsLoaded())
		{
			return;
		}
		std::lock_guard<std::mutex> lock(updateMutex_);
		cutoffShader_.Bind();
		cutoffShader_.SetBool("enableCutoff", enableCutoff_);
		cutoffShader_.SetMat4("view", camera_.GenerateViewMatrix());
		cutoffShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());

		//Draw grass
		cutoffShader_.SetTexture("texture1", grassTexture_, 0);
		const std::array<Vec3f, 5> vegetationPositions
		{
			Vec3f(-1.5f, 0.0f, -0.48f),
			Vec3f(1.5f, 0.0f, 0.51f),
			Vec3f(0.0f, 0.0f, 0.7f),
			Vec3f(-0.3f, 0.0f, -2.3f),
			Vec3f(0.5f, 0.0f, -0.6f)
		};
		for (const auto& position : vegetationPositions)
		{
			auto model = Mat4f::Identity;
			model = Transform3d::Translate(model, position);
			cutoffShader_.SetMat4("model", model);
			plane_.Draw();
		}
		//Draw cube
		cutoffShader_.SetTexture("texture1", cubeTexture_, 0);

		auto model = Mat4f::Identity;
		model = Transform3d::Translate(model, Vec3f(-1.0f, 0.0f, -1.0f));
		cutoffShader_.SetMat4("model", model);

		cube_.Draw();
		model = Mat4f::Identity;
		model = Transform3d::Translate(model, Vec3f(2.0f, 0.0f, 0.0f));
		cutoffShader_.SetMat4("model", model);
		cube_.Draw();
		
		//Draw floor

		cutoffShader_.SetTexture("texture1", whiteTexture_, 0);
		model = Mat4f::Identity;
		model = Transform3d::Rotate(model, Quaternion::AngleAxis(degree_t(-90.0f), Vec3f::right));
		model = Transform3d::Scale(model, Vec3f::one*5.0f);
		model = Transform3d::Translate(model, Vec3f::down*0.5f);
		cutoffShader_.SetMat4("model", model);
		plane_.Draw();
		
		
	}

	void HelloCutoffProgram::OnEvent(const SDL_Event& event)
	{
		camera_.OnEvent(event);
	}
}
