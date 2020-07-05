#pragma once
#include "comp_graph/sample_program.h"
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloPbrTextureProgram : public SampleProgram
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
		Vec3f color;
	};
	std::array<Light, 4> lights_;
	gl::RenderSphere sphere_{ Vec3f::zero, 1.0f };
	gl::Shader pbrShader_;
	sdl::Camera3D camera_;

	gl::Texture albedo_;
	gl::Texture normal_;
	gl::Texture metallic_;
	gl::Texture roughness_;
	unsigned int ao_ = 0;


	bool gammaCorrect_ = true;
};
}
