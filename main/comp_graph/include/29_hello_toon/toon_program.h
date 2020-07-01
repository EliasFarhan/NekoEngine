#pragma once
#include "comp_graph/sample_program.h"
#include "sdl_engine/sdl_camera.h"
#include "gl/model.h"

namespace neko
{
class HelloToonShadingProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	struct Light
	{
		Vec3f position;
		Vec3f color = Vec3f::one;
	};
	float dt_ = 0.0f;
	Light light_;
	int toonLayers_ = 4;
	sdl::Camera3D camera_;
	assimp::Model model_;
	gl::Shader toonShader_;
	gl::Shader blinnShader_;
};
}
