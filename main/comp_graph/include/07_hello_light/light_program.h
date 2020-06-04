#pragma once
#include "comp_graph/sample_program.h"
#include "gl/shader.h"
#include "gl/shape.h"
#include "graphics/color.h"
#include "sdl_engine/sdl_camera.h"


namespace neko
{
class HelloLightProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	gl::RenderCuboid cube_{ Vec3f::zero, Vec3f::one };
	sdl::Camera3D camera_;

	
	gl::Shader lightShader_;
	gl::Shader phongShader_;

	Vec3f lightPos_;

	float time_ = 0.0f;
	const float lightDist_ = 3.5f;
	Color3 lightColor_{ 1,1,1 };
	Color3 objectColor_{ 1.0f,0.23f, 0.1f };
	float ambientStrength_ = 0.1f;
	float diffuseStrength_ = 1.0f;
	float specularStrength_ = 0.5f;
	int specularPow_ = 32;
	
};
}
