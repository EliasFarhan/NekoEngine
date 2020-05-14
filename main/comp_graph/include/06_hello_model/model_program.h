#pragma once
#include "comp_graph/sample_program.h"
#include "sdl_engine/sdl_camera.h"
#include "gl/model.h"

namespace neko
{
class HelloModelProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	sdl::Camera3D camera_;
	assimp::Model model_;
	gl::Shader shader_;
	Mat4f projection_{ Mat4f::Identity };
};
}
