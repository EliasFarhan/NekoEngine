#include "09_hello_lightcasters/lightcasters_program.h"
#include "imgui.h"
#include "gl/texture.h"

namespace neko
{
void HelloLightCastersProgram::Init()
{
	for (auto& cubeAngle : cubeAngles)
	{
		cubeAngle = EulerAngles(
			degree_t(RandomRange(0.0f, 180.0f)),
			degree_t(RandomRange(0.0f, 180.0f)),
			degree_t(RandomRange(0.0f, 180.0f))
		);
	}

	const auto& config = BasicEngine::GetInstance()->config;
	containerDiffuse_ = gl::stbCreateTexture(config.dataRootPath + "/data/material/container2.png");
	containerSpecular_ = gl::stbCreateTexture(config.dataRootPath + "/data/material/container2_specular.png");
	containerShaders_[0].LoadFromFile(
		config.dataRootPath + "data/shaders/09_hello_lightcasters/container.vert",
		config.dataRootPath + "data/shaders/09_hello_lightcasters/container_directional.frag");
	containerShaders_[1].LoadFromFile(
		config.dataRootPath + "data/shaders/09_hello_lightcasters/container.vert",
		config.dataRootPath + "data/shaders/09_hello_lightcasters/container_point.frag");
	containerShaders_[2].LoadFromFile(
		config.dataRootPath + "data/shaders/09_hello_lightcasters/container.vert",
		config.dataRootPath + "data/shaders/09_hello_lightcasters/container_flash.frag");
	containerShaders_[3].LoadFromFile(
		config.dataRootPath + "data/shaders/09_hello_lightcasters/container.vert",
		config.dataRootPath + "data/shaders/09_hello_lightcasters/container_spot.frag");

	lampShader_.LoadFromFile(
		config.dataRootPath + "data/shaders/07_hello_light/lamp.vert",
		config.dataRootPath + "data/shaders/07_hello_light/lamp.frag"
	);
	cube_.Init();
	camera_.Init();
}

void HelloLightCastersProgram::Update(seconds dt)
{

	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);;
	camera_.Update(dt);
}

void HelloLightCastersProgram::Destroy()
{
	gl::DestroyTexture(containerDiffuse_);
	gl::DestroyTexture(containerSpecular_);
	for (auto& containerShader : containerShaders_)
	{
		containerShader.Destroy();
	}
	cube_.Destroy();
}

void HelloLightCastersProgram::DrawImGui()
{
	ImGui::Begin("Lightmaps Program");
	ImGui::InputFloat("ambientStrength", &ambientStrength_);
	ImGui::InputFloat("diffuseStrength", &diffuseStrength_);
	ImGui::InputFloat("specularStrength", &specularStrength_);
	ImGui::InputInt("specularPow", &specularPow_);
	switch(casterType_)
	{
	case LightCasterType::DIRECTIONAL:
	{
		ImGui::InputFloat3("Light Direction", &lightDirection_[0]);
		break;
	}
	case LightCasterType::POINT: break;
	case LightCasterType::FLASH:
	{
		float angle = lightCutOffAngle_.value();
		if(ImGui::InputFloat("Light CutOff", &angle))
		{
			lightCutOffAngle_ = degree_t(angle);
		}
		break;
	}
	case LightCasterType::SPOT: break;
	default: ;
	}
	ImGui::End();
}

void HelloLightCastersProgram::Render()
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const Mat4f view = camera_.GenerateViewMatrix();
	const Mat4f projection = camera_.GenerateProjectionMatrix();
	//Render lamp only if point
	if (casterType_ == LightCasterType::POINT)
	{
		lampShader_.Bind();
		Mat4f model = Mat4f::Identity;
		model = Transform3d::Scale(model, Vec3f(0.2f, 0.2f, 0.2f));
		model = Transform3d::Translate(model, lightPointPos_);
		lampShader_.SetMat4("model", model);
		lampShader_.SetMat4("view", view);
		lampShader_.SetMat4("projection", projection);

		lampShader_.SetVec3("lightColor", Vec3f(1, 1, 1));
		cube_.Draw();
	}
	//Render container cube
	Mat4f model = Mat4f::Identity;
	gl::Shader& containerShader = containerShaders_[(int)casterType_];
	containerShader.Bind();
	containerShader.SetMat4("model", model);
	containerShader.SetMat4("view", view);
	containerShader.SetMat4("projection", projection);
	switch (casterType_)
	{
	case LightCasterType::DIRECTIONAL:
	{
		containerShader.SetVec3("light.color", Vec3f(1, 1, 1));
		containerShader.SetVec3("light.direction", lightDirection_);
		break;
	}
	case LightCasterType::POINT:
	{
		containerShader.SetVec3("light.color", Vec3f(1, 1, 1));
		containerShader.SetVec3("light.position", lightPointPos_);
		break;
	}
	case LightCasterType::FLASH: {
		containerShader.SetVec3("light.color", Vec3f(1, 1, 1));
		containerShader.SetVec3("light.position", camera_.position);
		containerShader.SetVec3("light.direction", Vec3f::zero - camera_.reverseDirection);
		containerShader.SetFloat("light.cutOff", Cos(lightCutOffAngle_));
			break;
	}
	case LightCasterType::SPOT:
	{
		containerShader.SetVec3("light.color", Vec3f(1, 1, 1));
		containerShader.SetVec3("light.position", camera_.position);
		containerShader.SetVec3("light.direction", Vec3f::zero - camera_.reverseDirection);
		break;
	}
	default: break;;
	}


	containerShader.SetVec3("viewPos", camera_.position);

	containerShader.SetInt("objectMaterial.diffuse", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, containerDiffuse_);
	containerShader.SetInt("objectMaterial.specular", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, containerSpecular_);
	containerShader.SetInt("objectMaterial.shininess", specularPow_);

	containerShader.SetFloat("ambientStrength", ambientStrength_);
	containerShader.SetFloat("diffuseStrength", diffuseStrength_);
	containerShader.SetFloat("specularStrength", specularStrength_);

	const auto inverseTransposeModel = model.Inverse().Transpose();
	containerShader.SetMat4("inverseTransposeModel", inverseTransposeModel);

	cube_.Draw();
}

void HelloLightCastersProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
