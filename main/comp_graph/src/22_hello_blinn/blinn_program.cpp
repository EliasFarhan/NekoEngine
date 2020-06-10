#include "22_hello_blinn/blinn_program.h"
#include "imgui.h"

namespace neko
{
void HelloBlinnProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	modelShader_.LoadFromFile(config.dataRootPath + "shaders/22_hello_blinn/model.vert",
		config.dataRootPath + "shaders/22_hello_blinn/model.frag");
	blinnShader_.LoadFromFile(config.dataRootPath + "shaders/22_hello_blinn/model.vert",
		config.dataRootPath + "shaders/22_hello_blinn/model_blinn.frag");
	floor_.Init();
	floorTexture_.SetTextureFlags(gl::Texture::TextureFlags(gl::Texture::REPEAT_WRAP | gl::Texture::DEFAULT));
	floorTexture_.SetPath(config.dataRootPath + "sprites/brickwall/brickwall.jpg");
	floorTexture_.LoadFromDisk();
	model_.LoadModel(config.dataRootPath + "model/nanosuit2/nanosuit.obj");

	camera_.position = Vec3f(0.0f, 3.0f, 3.0f);
	camera_.LookAt(Vec3f::zero);
}

void HelloBlinnProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
	dt_ += dt.count();
	lightPos_ = Vec3f(3.0f*Cos(radian_t(dt_)), 3.0f, 3.0f*Sin(radian_t(dt_)));
}

void HelloBlinnProgram::Destroy()
{
	modelShader_.Destroy();
	blinnShader_.Destroy();
	floor_.Destroy();
	floorTexture_.Destroy();
	model_.Destroy();
}

void HelloBlinnProgram::DrawImGui()
{
	ImGui::Begin("Blinn Program");
	bool enableBlinn = flags_ & ENABLE_BLINN;
	if(ImGui::Checkbox("Enable Blinn", &enableBlinn))
	{
		flags_ = enableBlinn ? flags_ | ENABLE_BLINN : flags_ & ~ENABLE_BLINN;
	}
	ImGui::SliderFloat("Floor Texture Resolution", &floorResolution_, 0.5f, 12.0f);
	ImGui::SliderInt("Specular Pow", &specularPow_, 1, 256);
	bool onlySpecular = flags_ & ONLY_SPECULAR;
	if(ImGui::Checkbox("Only Specular", &onlySpecular))
	{
		flags_ = onlySpecular ? flags_ | ONLY_SPECULAR : flags_ & ~ONLY_SPECULAR;
	}
	ImGui::End();
}

void HelloBlinnProgram::Render()
{
	if(!model_.IsLoaded())
		return;
	if (!floorTexture_.IsLoaded())
		return;
	std::lock_guard<std::mutex> lock(updateMutex_);

	auto& shader = flags_ & ENABLE_BLINN ? blinnShader_ : modelShader_;
	shader.Bind();
	shader.SetFloat("light.constant", 1.0f);
	shader.SetFloat("light.linear", 0.09f);
	shader.SetFloat("light.quadratic", 0.032f);
	shader.SetMat4("view", camera_.GenerateViewMatrix());
	shader.SetMat4("projection", camera_.GenerateProjectionMatrix());
	auto model = Mat4f::Identity;
	model = Transform3d::Scale(model, Vec3f(0.1f, 0.1f, 0.1f));
	shader.SetMat4("model", model);
	shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
	shader.SetVec3("light.position", lightPos_);
	shader.SetVec3("viewPos", camera_.position);
	shader.SetFloat("material.resolution", 1.0f);
	shader.SetFloat("material.shininess", 32.0f);
	shader.SetBool("onlySpecular", flags_ & ONLY_SPECULAR);
	model_.Draw(shader);

	model = Mat4f::Identity;
	model = Transform3d::Rotate(model, degree_t(-90.0f), Vec3f::right);
	shader.SetMat4("model", model);
	shader.SetMat4("transposeInverseModel", model.Inverse().Transpose());
	shader.SetInt("material.texture_diffuse1", 0);
	shader.SetFloat("material.resolution", floorResolution_);

	shader.SetFloat("material.shininess", static_cast<float>(specularPow_));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorTexture_.GetTextureId());
	floor_.Draw();
}

void HelloBlinnProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
