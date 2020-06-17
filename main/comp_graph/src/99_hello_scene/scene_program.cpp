#include "99_hello_scene/scene_program.h"
#include "imgui.h"

namespace neko
{
HelloSceneProgram::HelloSceneProgram() :
	entityHierarchy_(entityManager_),
	transform3dManager_(entityManager_),
	entityViewer_(entityManager_, entityHierarchy_),
	transformViewer_(entityManager_, transform3dManager_)
{
}

void HelloSceneProgram::Init()
{
	camera_.Init();
	transform3dManager_.Init();
	for(int i = 0; i < initEntityNmb_; i++)
	{
		const auto entity = entityManager_.CreateEntity();
		transform3dManager_.AddComponent(entity);
		transform3dManager_.SetPosition(entity, Vec3f(
			RandomRange(-10.0f, 10.0f),
			RandomRange(-10.0f, 10.0f),
			RandomRange(0.0f, 10.0f)
		));
		transform3dManager_.SetScale(entity, Vec3f::one * RandomRange(0.5f, 3.0f));
		transform3dManager_.SetRotation(entity, EulerAngles(
			degree_t(RandomRange(0.0f,90.0f)),
			degree_t(RandomRange(0.0f, 90.0f)),
			degree_t(RandomRange(0.0f, 90.0f))
		));
	}
	const auto& config = BasicEngine::GetInstance()->config;
    shader_.LoadFromFile(config.dataRootPath + "shaders/99_hello_scene/shadow.vert",
                         config.dataRootPath + "shaders/99_hello_scene/cube.frag");
	cube_.Init();
}

void HelloSceneProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	transform3dManager_.Update();
	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
}

void HelloSceneProgram::Destroy()
{
	cube_.Destroy();
	shader_.Destroy();
}

void HelloSceneProgram::DrawImGui()
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	entityViewer_.DrawImGui();

	transformViewer_.SetSelectedEntity(entityViewer_.GetSelectedEntity());
	ImGui::Begin("Inspector");
	transformViewer_.DrawImGui();
	ImGui::End();
}

void HelloSceneProgram::Render()
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	shader_.Bind();
	shader_.SetMat4("view",camera_.GenerateViewMatrix());
	shader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
	const auto selectedEntity = entityViewer_.GetSelectedEntity();
	for(Entity entity = 0; entity < entityManager_.GetEntitiesSize();entity++)
	{
		shader_.SetVec3("color", 
			selectedEntity == entity ? Color3(0, 1, 0) : Color3(1, 0.3f, 0.2f));
		if (entityManager_.EntityExists(entity))
		{
			const Mat4f model = transform3dManager_.GetComponent(entity);
			shader_.SetMat4("model", model);
			cube_.Draw();
		}
	}
}

void HelloSceneProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
