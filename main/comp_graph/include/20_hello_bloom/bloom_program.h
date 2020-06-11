#pragma once

#include "comp_graph/sample_program.h"

#include "gl/shader.h"
#include "gl/shape.h"
#include "gl/texture.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloBloomProgram : public SampleProgram
{
public:
	void OnEvent(const SDL_Event& event) override;

	void Init() override;

	void Update(seconds dt) override;

	void Destroy() override;

	void DrawImGui() override;

	void Render() override;
private:
	enum BloomFlags : std::uint8_t
	{
		NONE = 0u,
		ENABLE_BLOOM = 1u,
		RESIZE_FRAMEBUFFER = 1u << 1u
	};
	struct Light
	{
		Vec3f position_;
		Vec3f color_;
	};
	struct Transform
	{
		Vec3f position = Vec3f::zero;
		Vec3f scale = Vec3f::one;
		radian_t angle = radian_t(0.0f);
		Vec3f axis = Vec3f::up;
	};

	void CreateFramebuffer();
	
	gl::Shader cubeShader_;
	gl::Shader lightShader_;
	gl::Shader blurShader_;
	gl::Shader bloomShader_;

	gl::Texture cubeTexture_;
	gl::RenderCuboid cube_{ Vec3f::zero, Vec3f::one };

	std::array<Transform, 6> cubeTransforms_ = {
		{
			{Vec3f(0.0f, 1.5f, 0.0),Vec3f(0.5f)},
			{Vec3f(2.0f, 0.0f, 1.0), Vec3f(0.5f)},
			{Vec3f(-1.0f, -1.0f, 2.0),Vec3f::one,
				degree_t(60.0f), Vec3f(1,0,1).Normalized()},
			{Vec3f(0.0f, 2.7f, 4.0), Vec3f(1.25f),
				degree_t(23.0f),Vec3f(1.0, 0.0, 1.0).Normalized()},
			{Vec3f(-2.0f, 1.0f, -3.0),Vec3f::one,
				degree_t(124.0f),Vec3f(1.0, 0.0, 1.0).Normalized()},
			{Vec3f(-3.0f, 0.0f, 0.0),Vec3f(0.5f)}

		} };
	std::array<Light, 4> lights_ = {
		{
			{Vec3f(0.0f, 0.5f,  1.5f), Color3(5.0f,   5.0f,  5.0f)},
			{Vec3f(-4.0f, 0.5f, -3.0f), Color3{10.0f,  0.0f,  0.0f}},
			{Vec3f(3.0f, 0.5f,  1.0f), Color3(0.0f,   0.0f,  15.0f)},
			{Vec3f(-.8f,  2.4f, -1.0f), Color3(0.0f,   5.0f,  0.0f)}
		}
	};

	sdl::Camera3D camera_;

	unsigned int hdrFbo_ = 0;
	unsigned int colorBuffers_[2]{};
	unsigned int rbo_ = 0;

	unsigned int pingpongFbo_[2]{};
	unsigned int pingpongColorBuffers_[2]{};
	int blurAmount_ = 10;
	gl::RenderQuad screenPlane_{Vec3f::zero, Vec2f::one*2.0f};
	std::uint8_t flags_ = NONE;
	float exposure_ = 1.0f;
};
}