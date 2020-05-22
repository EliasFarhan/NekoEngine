#pragma once
#include "comp_graph/sample_program.h"
#include "engine/entity.h"
#include "engine/transform.h"
#include "gl/shape.h"
#include "gl/shader.h"

namespace neko
{

class HelloSceneProgram : public SampleProgram
{
public:
	HelloSceneProgram();
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	//Scene system
	EntityManager entityManager_;
	EntityHierarchy entityHierarchy_;
	Transform3dManager transform3dManager_;
	//ImGui Viewer
	EntityViewer entityViewer_;
	Transform3dViewer transformViewer_;

	//Initialization data
	const size_t initEntityNmb_ = 10;
	gl::RenderCuboid cube_{Vec3f::zero, Vec3f::one/2.0f};
	gl::Shader shader_;
	sdl::Camera3D camera_;
	const EntityMask cubeComponentType = EntityMask(ComponentType::OTHER_TYPE);
	
};
};
