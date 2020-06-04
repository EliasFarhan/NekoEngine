#include "14_hello_outline/outline_program.h"
#include "imgui.h"

namespace neko
{
void HelloOutlineProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	cube_.Init();
	cubeTexture_.SetPath(config.dataRootPath + "sprites/container.jpg");
	cubeTexture_.LoadFromDisk();

	modelShader_.LoadFromFile(config.dataRootPath + "shaders/14_hello_outline/model.vert",
		config.dataRootPath + "shaders/14_hello_outline/model.frag");
	outlineShader_.LoadFromFile(config.dataRootPath + "shaders/14_hello_outline/outline.vert", 
		config.dataRootPath + "shaders/14_hello_outline/outline.frag");
	camera_.position = Vec3f(0.0f, 3.0f, 3.0f);
	camera_.LookAt(Vec3f::zero);
}

void HelloOutlineProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
}

void HelloOutlineProgram::Destroy()
{
	cube_.Destroy();
	cubeTexture_.Destroy();
	modelShader_.Destroy();
	outlineShader_.Destroy();
}

void HelloOutlineProgram::DrawImGui()
{
	ImGui::Begin("Outline Program");
	ImGui::SliderFloat("Outline Scale", &outlineScale_, 1.0f, 1.5f);
	ImGui::ColorPicker3("Outline Color", &outlineColor_[0]);
	bool drawOutline = flags_ & DRAW_OUTLINE;
	if(ImGui::Checkbox("Draw Outline", &drawOutline))
	{
		flags_ = drawOutline ? flags_ | DRAW_OUTLINE : flags_ & ~DRAW_OUTLINE;
	}
	bool useStencil = flags_ & USE_STENCIL;
	if(ImGui::Checkbox("Use Stencil", &useStencil))
	{
		flags_ = useStencil ? flags_ | USE_STENCIL : flags_ & ~USE_STENCIL;
	}
	
	ImGui::End();
}

void HelloOutlineProgram::Render()
{
	if (!cubeTexture_.IsLoaded())
		return;

	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto view = camera_.GenerateViewMatrix();
	const auto projection = camera_.GenerateProjectionMatrix();

	//Draw cube
	if(flags_ & USE_STENCIL)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //write only when depth pass
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
		glStencilMask(0xFF); // enable writing to the stencil buffer
		glClear(GL_STENCIL_BUFFER_BIT); //set stencil buffer values to 0
	}
	modelShader_.Bind();
	modelShader_.SetMat4("view", view);
	modelShader_.SetMat4("projection", projection);
	modelShader_.SetMat4("model", Mat4f::Identity);

	modelShader_.SetInt("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTexture_.GetTextureId());

	cube_.Draw();

	if(flags_ & USE_STENCIL)
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //only draw if stencil value is not one
		glStencilMask(0x00); // disable writing to the stencil buffer
	}
	if(flags_ & DRAW_OUTLINE)
	{
		outlineShader_.Bind();
		outlineShader_.SetMat4("view", view);
		outlineShader_.SetMat4("projection", projection);
		auto model = Mat4f::Identity;
		model = Transform3d::Scale(model, Vec3f::one * outlineScale_);
		outlineShader_.SetMat4("model", model);
		outlineShader_.SetVec3("color", outlineColor_);

		cube_.Draw();
	}
	if(flags_ & USE_STENCIL)
	{
		glDisable(GL_STENCIL_TEST);
	}
}

void HelloOutlineProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
