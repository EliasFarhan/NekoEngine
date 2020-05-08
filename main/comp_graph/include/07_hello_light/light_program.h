#pragma once
#include "comp_graph/sample_program.h"
#include "gl/shader.h"
#include "gl/shape.h"
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
	Color lightColor_{ 1,1,1,1 };
	Color objectColor_{ 1.0f,0.23f, 0.1f,1.0f };

	gl::RenderCuboid cube_;
	sdl::Camera camera_;

	Mat4f projection_{ Mat4f::Identity };
	
	gl::Shader lightShader_;
	gl::Shader phongShader_;

	float time_ = 0.0f;
	
};
}
