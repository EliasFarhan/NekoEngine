#include "08_hello_lightmaps/lightmaps_program.h"
#include "gl/texture.h"
#include "imgui.h"

namespace neko
{
void HelloLightMapsProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	containerDiffuse_ = gl::stbCreateTexture(config.dataRootPath + "material/container2.png");
	containerSpecular_ = gl::stbCreateTexture(config.dataRootPath + "material/container2_specular.png");
    containerShader_.LoadFromFile(
            config.dataRootPath + "shaders/08_hello_lightmaps/container.vert",
            config.dataRootPath + "shaders/08_hello_lightmaps/container.frag");
    lampShader_.LoadFromFile(
            config.dataRootPath + "shaders/07_hello_light/lamp.vert",
            config.dataRootPath + "shaders/07_hello_light/lamp.frag");
	cube_.Init();
	camera_.Init();
}

void HelloLightMapsProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	time_ += dt.count();
	lightPos_ = Vec3f(
		Cos(radian_t(time_)),
		0.0f,
		Sin(radian_t(time_))) * lightDist_;
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);;
	camera_.Update(dt);
}

void HelloLightMapsProgram::Destroy()
{
	gl::DestroyTexture(containerDiffuse_);
	gl::DestroyTexture(containerSpecular_);
	containerShader_.Destroy();
	lampShader_.Destroy();
	cube_.Destroy();
}

void HelloLightMapsProgram::DrawImGui()
{
	ImGui::Begin("Lightmaps Program");
	ImGui::InputFloat("ambientStrength", &ambientStrength_);
	ImGui::InputFloat("diffuseStrength", &diffuseStrength_);
	ImGui::InputFloat("specularStrength", &specularStrength_);
	ImGui::InputInt("specularPow", &specularPow_);
	ImGui::End();
}

void HelloLightMapsProgram::Render()
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const Mat4f view = camera_.GenerateViewMatrix();
	const Mat4f projection = camera_.GenerateProjectionMatrix();
	//Render lamp
	lampShader_.Bind();
	Mat4f model = Mat4f::Identity;
	model = Transform3d::Scale(model, Vec3f(0.2f, 0.2f, 0.2f));
	model = Transform3d::Translate(model, lightPos_);
	lampShader_.SetMat4("model", model);
	lampShader_.SetMat4("view", view);
	lampShader_.SetMat4("projection", projection);
	 
	lampShader_.SetVec3("lightColor", Vec3f(1, 1, 1));
	cube_.Draw();
	//Render container cube
	containerShader_.Bind();
	model = Mat4f::Identity;
	containerShader_.SetMat4("model", model);
	containerShader_.SetMat4("view", view);
	containerShader_.SetMat4("projection", projection);

	containerShader_.SetVec3("light.color", Vec3f(1, 1, 1));
	containerShader_.SetVec3("light.position", lightPos_);

	containerShader_.SetVec3("viewPos", camera_.position);

	containerShader_.SetInt("objectMaterial.diffuse", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, containerDiffuse_);
	containerShader_.SetInt("objectMaterial.specular", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, containerSpecular_);
	containerShader_.SetInt("objectMaterial.shininess", specularPow_);

	containerShader_.SetFloat("ambientStrength", ambientStrength_);
	containerShader_.SetFloat("diffuseStrength", diffuseStrength_);
	containerShader_.SetFloat("specularStrength", specularStrength_);
	
	const auto inverseTransposeModel = model.Inverse().Transpose();
	containerShader_.SetMat4("inverseTransposeModel", inverseTransposeModel);
	
	cube_.Draw();
	
}

void HelloLightMapsProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
