#include "18_hello_normal/normal_program.h"
#include "imgui.h"

namespace neko
{

void HelloNormalProgram::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	diffuseTex_.SetPath(config.dataRootPath + "sprites/brickwall/brickwall.jpg");
	diffuseTex_.LoadFromDisk();
	normalTex_.SetPath(config.dataRootPath + "sprites/brickwall/brickwall_normal.jpg");
	normalTex_.LoadFromDisk();

    normalShader_.LoadFromFile(
            config.dataRootPath + "shaders/18_hello_normal/normal.vert",
            config.dataRootPath + "shaders/18_hello_normal/normal.frag");
    diffuseShader_.LoadFromFile(
            config.dataRootPath + "shaders/18_hello_normal/model.vert",
            config.dataRootPath + "shaders/18_hello_normal/model.frag");

	plane_.Init();
	cube_.Init();
	model_.LoadModel(config.dataRootPath + "model/nanosuit2/nanosuit.obj");

	camera_.position = Vec3f(-3.0f, 3.0f, 3.0f);
	camera_.LookAt(Vec3f::zero);
}

void HelloNormalProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->config;
	dt_ += dt.count();
	lightPos_ = Vec3f(Cos(radian_t(dt_)), 1.0f, Sin(radian_t(dt_))) * 3.0f;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
}

void HelloNormalProgram::Destroy()
{
	model_.Destroy();
	normalShader_.Destroy();
	plane_.Destroy();
	cube_.Destroy();
	model_.Destroy();
	diffuseTex_.Destroy();
	normalTex_.Destroy();
}

void HelloNormalProgram::DrawImGui()
{
	ImGui::Begin("Normal Program");
	bool useModel = flags_ & ENABLE_MODEL;
	if (ImGui::Checkbox("Use Model", &useModel))
	{
		flags_ = useModel ? flags_ | ENABLE_MODEL : flags_ & ~ENABLE_MODEL;
	}
	bool usePlane = flags_ & ENABLE_PLANE;
	if (ImGui::Checkbox("Use Plane", &usePlane))
	{
		flags_ = usePlane ? flags_ | ENABLE_PLANE : flags_ & ~ENABLE_PLANE;
	}
	bool useCube = flags_ & ENABLE_CUBE;
	if (ImGui::Checkbox("Use Cube", &useCube))
	{
		flags_ = useCube ? flags_ | ENABLE_CUBE : flags_ & ~ENABLE_CUBE;
	}
	bool enableNormal = flags_ & ENABLE_NORMAL_MAP;
	if (ImGui::Checkbox("Enable Normal Map", &enableNormal))
	{
		flags_ = enableNormal ? flags_ | ENABLE_NORMAL_MAP : flags_ & ~ENABLE_NORMAL_MAP;
	}
	ImGui::End();
}

void HelloNormalProgram::Render()
{
	if (!model_.IsLoaded())
		return;
	if (!normalTex_.IsLoaded())
		return;
	if (!diffuseTex_.IsLoaded())
		return;
	std::lock_guard<std::mutex> lock(updateMutex_);

	const std::function<void(NormalFlags)> draw = [this](NormalFlags flag)
	{
		auto model = Mat4f::Identity;
		if (flag == ENABLE_MODEL)
		{
			model = Transform3d::Scale(model, Vec3f::one * 0.1f);
		}
		else
		{
			model = Transform3d::Scale(model, Vec3f::one * 3.0f);
		}
		const auto transposeInverseModel = model.Inverse().Transpose();
		if (flags_ & ENABLE_NORMAL_MAP)
		{
			normalShader_.Bind();
			normalShader_.SetVec3("viewPos", camera_.position);
			normalShader_.SetMat4("view", camera_.GenerateViewMatrix());
			normalShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
			normalShader_.SetVec3("lightPos", lightPos_);
			normalShader_.SetMat4("model", model);

			normalShader_.SetMat4("transposeInverseModel", transposeInverseModel);
			normalShader_.SetBool("enableNormalMap", true);
			if (flag != ENABLE_MODEL)
			{
				normalShader_.SetInt("material.texture_diffuse1", 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, diffuseTex_.GetTextureId());
				normalShader_.SetInt("material.texture_normal1", 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, normalTex_.GetTextureId());
			}
		}
		else
		{
			diffuseShader_.Bind();
			diffuseShader_.SetVec3("viewPos", camera_.position);
			diffuseShader_.SetVec3("lightPos", lightPos_);

			diffuseShader_.SetMat4("view", camera_.GenerateViewMatrix());
			diffuseShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
			diffuseShader_.SetMat4("model", model);
			diffuseShader_.SetMat4("transposeInverseModel", transposeInverseModel);
			if (flag != ENABLE_MODEL)
			{
				diffuseShader_.SetInt("material.texture_diffuse1", 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, diffuseTex_.GetTextureId());
			}
		}
		switch (flag)
		{
		case ENABLE_PLANE:
			plane_.Draw();
			break;
		case ENABLE_MODEL:
			model_.Draw(flags_ & ENABLE_NORMAL_MAP ? normalShader_ : diffuseShader_);
			break;
		case ENABLE_CUBE:
			cube_.Draw();
			break;
		default:
			break;
		}
	};
	if(flags_ & ENABLE_MODEL)
	{
		draw(ENABLE_MODEL);
	}
	if (flags_ & ENABLE_CUBE)
	{
		draw(ENABLE_CUBE);
	}
	if (flags_ & ENABLE_PLANE)
	{
		draw(ENABLE_PLANE);
	}
	
}

void HelloNormalProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}

}