#include "15_hello_cubemaps/cubemaps_program.h"
#include "imgui.h"

namespace neko
{
void HelloCubemapsProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	skyboxCube_.Init();
	skyboxTexture_ = gl::LoadCubemap({
        config.dataRootPath+"sprites/skybox/right.jpg",
		config.dataRootPath+"sprites/skybox/left.jpg",
		config.dataRootPath+"sprites/skybox/top.jpg",
		config.dataRootPath+"sprites/skybox/bottom.jpg",
		config.dataRootPath+"sprites/skybox/front.jpg",
		config.dataRootPath+"sprites/skybox/back.jpg"
	});
	skyboxShader_.LoadFromFile(
		config.dataRootPath + "shaders/15_hello_cubemaps/skybox.vert",
		config.dataRootPath + "shaders/15_hello_cubemaps/skybox.frag"
	);
	model_.LoadModel(config.dataRootPath + "model/nanosuit2/nanosuit.obj");
	modelShader_.LoadFromFile(config.dataRootPath + "shaders/15_hello_cubemaps/model.vert",
		config.dataRootPath + "shaders/15_hello_cubemaps/model.frag");
	modelReflectionShader_.LoadFromFile(config.dataRootPath + "shaders/15_hello_cubemaps/model.vert",
		config.dataRootPath + "shaders/15_hello_cubemaps/model_reflection.frag");
	modelRefractionShader_.LoadFromFile(config.dataRootPath + "shaders/15_hello_cubemaps/model.vert",
		config.dataRootPath + "shaders/15_hello_cubemaps/model_refraction.frag");
	camera_.position = Vec3f(0, 3, 3);
	camera_.LookAt(Vec3f::zero);
	cube_.Init();
	cubeTexture_.SetPath(config.dataRootPath + "sprites/container.jpg");
	cubeTexture_.LoadFromDisk();
}

void HelloCubemapsProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
}

void HelloCubemapsProgram::Destroy()
{
	glDeleteTextures(1, &skyboxTexture_);
	skyboxCube_.Destroy();
	skyboxShader_.Destroy();

	modelShader_.Destroy();
	modelReflectionShader_.Destroy();
	modelRefractionShader_.Destroy();
	cube_.Destroy();
	model_.Destroy();
}

void HelloCubemapsProgram::DrawImGui()
{
	ImGui::Begin("Hello Cubemaps");
	const char* renderModeNames_[static_cast<int>(ModelRenderMode::LENGTH)] =
	{
		"None",
		"Reflection",
		"Refraction"
	};
	int currentItem = static_cast<int>(currentRenderMode_);
	if(ImGui::Combo("Render Mode", &currentItem, renderModeNames_, static_cast<int>(ModelRenderMode::LENGTH)))
	{
		currentRenderMode_ = static_cast<ModelRenderMode>(currentItem);
	}
	switch(currentRenderMode_)
	{
	case ModelRenderMode::REFLECTION:
		ImGui::SliderFloat("Reflection Value", &reflectionValue_, 0.0f, 1.0f);
		break;
	case ModelRenderMode::REFRACTION:
		ImGui::SliderFloat("Refraction Value", &refractionValue_, 0.0f, 1.0f);
		ImGui::SliderFloat("Refractive Index", &refractiveIndex_, 1.0f, 3.0f);
		break;
	default:
		break;
	}
	ImGui::End();
}

void HelloCubemapsProgram::Render()
{
	if(!model_.IsLoaded())
	{
		return;
	}
	if(!cubeTexture_.IsLoaded())
	{
		return;
	}
	std::lock_guard<std::mutex> lock(updateMutex_);

	const auto view = camera_.GenerateViewMatrix();
	const auto projection = camera_.GenerateProjectionMatrix();
	//Draw model
	switch(currentRenderMode_)
	{
	case ModelRenderMode::NONE:
	{
		modelShader_.Bind();
		modelShader_.SetMat4("view", view);
		modelShader_.SetMat4("projection", projection);
		auto model = Mat4f::Identity;
		model = Transform3d::Scale(model, Vec3f(0.1f, 0.1f, 0.1f));
		modelShader_.SetMat4("model", model);
		modelShader_.SetMat4("transposeInverseModel", model.Inverse().Transpose());
		model_.Draw(modelShader_);
		model = Mat4f::Identity;
		model = Transform3d::Translate(model, Vec3f::left * 2.0f);
		modelShader_.SetMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture_.GetTextureId());
		cube_.Draw();
		break;
	}
	case ModelRenderMode::REFLECTION:
	{
		modelReflectionShader_.Bind();
		modelReflectionShader_.SetMat4("view", view);
		modelReflectionShader_.SetMat4("projection", projection);
		auto model = Mat4f::Identity;
		model = Transform3d::Scale(model, Vec3f(0.1f, 0.1f, 0.1f));
		modelReflectionShader_.SetMat4("model", model);
		modelReflectionShader_.SetMat4("transposeInverseModel", model.Inverse().Transpose());
		modelReflectionShader_.SetInt("skybox", 2);
		modelReflectionShader_.SetVec3("cameraPos", camera_.position);
		modelReflectionShader_.SetFloat("reflectionValue", reflectionValue_);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture_);
		model_.Draw(modelReflectionShader_);
		model = Mat4f::Identity;
		model = Transform3d::Translate(model, Vec3f::left * 2.0f);
		modelReflectionShader_.SetMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture_.GetTextureId());
		cube_.Draw();
		break;
	}
	case ModelRenderMode::REFRACTION:
	{
		modelRefractionShader_.Bind();
		modelRefractionShader_.SetMat4("view", view);
		modelRefractionShader_.SetMat4("projection", projection);
		auto model = Mat4f::Identity;
		model = Transform3d::Scale(model, Vec3f(0.1f, 0.1f, 0.1f));
		modelRefractionShader_.SetMat4("model", model);
		modelRefractionShader_.SetMat4("transposeInverseModel", model.Inverse().Transpose());

		modelRefractionShader_.SetFloat("refractiveIndex", refractiveIndex_);
		modelRefractionShader_.SetFloat("refractionValue", refractionValue_);
		modelRefractionShader_.SetVec3("cameraPos", camera_.position);
		modelRefractionShader_.SetInt("skybox", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture_);
		model_.Draw(modelRefractionShader_);
		model = Mat4f::Identity;
		model = Transform3d::Translate(model, Vec3f::left * 2.0f);
		modelRefractionShader_.SetMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture_.GetTextureId());
		cube_.Draw();
		break;
	}
	default: ;
	}
	
	//Draw skybox
	glDepthFunc(GL_LEQUAL);
	skyboxShader_.Bind();
	skyboxShader_.SetMat4("view", Mat4f(view.ToMat3()));
	skyboxShader_.SetMat4("projection", projection);
	skyboxShader_.SetInt("skybox", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture_);
	skyboxCube_.Draw();
	glDepthFunc(GL_LESS);
}

void HelloCubemapsProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
