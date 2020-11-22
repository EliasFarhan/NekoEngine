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

#include "09_hello_lightcasters/lightcasters_program.h"
#include "imgui.h"
#include "gl/texture.h"

namespace neko
{
void HelloLightCastersProgram::Init()
{
	for (auto& cubeAngle : cubeAngles_)
	{
		cubeAngle = EulerAngles(
			degree_t(RandomRange(0.0f, 180.0f)),
			degree_t(RandomRange(0.0f, 180.0f)),
			degree_t(RandomRange(0.0f, 180.0f))
		);
	}

	const auto& config = BasicEngine::GetInstance()->GetConfig();
	const auto& filesystem = BasicEngine::GetInstance()->GetFilesystem();
	containerDiffuse_ = gl::StbCreateTexture(config.dataRootPath + "material/container2.png",
                                          filesystem);
	containerSpecular_ = gl::StbCreateTexture(config.dataRootPath + "material/container2_specular.png",
                                              filesystem);
    containerShaders_[0].LoadFromFile(
            config.dataRootPath + "shaders/09_hello_lightcasters/container.vert",
            config.dataRootPath + "shaders/09_hello_lightcasters/container_directional.frag");
    containerShaders_[1].LoadFromFile(
            config.dataRootPath + "shaders/09_hello_lightcasters/container.vert",
            config.dataRootPath + "shaders/09_hello_lightcasters/container_point.frag");
    containerShaders_[2].LoadFromFile(
            config.dataRootPath + "shaders/09_hello_lightcasters/container.vert",
            config.dataRootPath + "shaders/09_hello_lightcasters/container_flash.frag");
    containerShaders_[3].LoadFromFile(
            config.dataRootPath + "shaders/09_hello_lightcasters/container.vert",
            config.dataRootPath + "shaders/09_hello_lightcasters/container_spot.frag");

    lampShader_.LoadFromFile(
            config.dataRootPath + "shaders/07_hello_light/lamp.vert",
            config.dataRootPath + "shaders/07_hello_light/lamp.frag");
	cube_.Init();
	camera_.Init();
	camera_.position = Vec3f(15.0f, 12.0f, -14.0f);
	camera_.reverseDir = Vec3f(7, 5, -2).Normalized();
}

void HelloLightCastersProgram::Update(seconds dt)
{

	std::lock_guard<std::mutex> lock(updateMutex_);
	time_ += dt.count();
	lightPointPos_ = Vec3f(
		Cos(radian_t(time_)),
		0.0f,
		Sin(radian_t(time_))) * lightDist_;
	const auto& config = BasicEngine::GetInstance()->GetConfig();
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
	ImGui::Begin("Light Casters Program");
	ImGui::Text("Camera Position: %f, %f, %f", 
		camera_.position.x, camera_.position.y, camera_.position.z);
	ImGui::Text("Camera Direction: %f, %f, %f", 
		-camera_.reverseDir.x, -camera_.reverseDir.y, -camera_.reverseDir.z);
	ImGui::InputFloat("ambientStrength", &ambientStrength_);
	ImGui::InputFloat("diffuseStrength", &diffuseStrength_);
	ImGui::InputFloat("specularStrength", &specularStrength_);
	ImGui::InputInt("specularPow", &specularPow_);
	switch(casterType_)
	{
	case LightCasterType::DIRECTIONAL:
	{
		if(ImGui::InputFloat3("Light Direction", &lightDirection_[0]))
		{
			lightDirection_ = lightDirection_.Normalized();
		}
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
	case LightCasterType::SPOT:
	{
		float angle = lightCutOffAngle_.value();
		if (ImGui::InputFloat("Light CutOff", &angle))
		{
			lightCutOffAngle_ = degree_t(angle);
		}
		angle = lightOuterCutOffAngle_.value();
		if (ImGui::InputFloat("Light Outer CutOff", &angle))
		{
			lightOuterCutOffAngle_ = degree_t(angle);
		}
		break;
	}
	default: ;
	}
	//Select the Light Caster Type
	const char* casterTypeNames[] = {
		"Directional",
		"Point",
		"Flash",
		"Spot"
	};
	int casterTypeIndex = static_cast<int>(casterType_);
	if(ImGui::Combo("Light Caster Type", 
		&casterTypeIndex, 
		casterTypeNames, 
		static_cast<int>(LightCasterType::LENGTH)))
	{
		casterType_ = static_cast<LightCasterType>(casterTypeIndex);
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
	gl::Shader& containerShader = containerShaders_[static_cast<int>(casterType_)];
	containerShader.Bind();
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
		containerShader.SetFloat("light.constant", 1.0f);
		containerShader.SetFloat("light.linear", 0.09f);
		containerShader.SetFloat("light.quadratic", 0.032f);
		break;
	}
	case LightCasterType::FLASH: {
		containerShader.SetVec3("light.color", Vec3f(1, 1, 1));
		containerShader.SetVec3("light.position", camera_.position);
		containerShader.SetVec3("light.direction", Vec3f::zero - camera_.reverseDir);
		containerShader.SetFloat("light.cutOff", Cos(lightCutOffAngle_));
		break;
	}
	case LightCasterType::SPOT:
	{
		containerShader.SetVec3("light.color", Vec3f(1, 1, 1));
		containerShader.SetVec3("light.position", camera_.position);
		containerShader.SetVec3("light.direction", Vec3f::zero - camera_.reverseDir);
		containerShader.SetFloat("light.cutOff", Cos(lightCutOffAngle_));
		containerShader.SetFloat("light.outerCutOff", Cos(lightOuterCutOffAngle_));
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
	for (size_t i = 0; i < cubeNumbers_; i++)
	{

		Mat4f model = Mat4f::Identity;
		model = Transform3d::Rotate(model, cubeAngles_[i].x, Vec3f::left);
		model = Transform3d::Rotate(model, cubeAngles_[i].y, Vec3f::up);
		model = Transform3d::Rotate(model, cubeAngles_[i].z, Vec3f::forward);
		model = Transform3d::Translate(model, cubePositions_[i]);
		containerShader.SetMat4("model", model);

		const auto inverseTransposeModel = model.Inverse().Transpose();
		containerShader.SetMat4("inverseTransposeModel", inverseTransposeModel);

		cube_.Draw();
	}
}

void HelloLightCastersProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
