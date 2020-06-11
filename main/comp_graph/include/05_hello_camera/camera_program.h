#pragma once
#include "comp_graph/sample_program.h"
#include "gl/shader.h"
#include "gl/shape.h"
#include "gl/texture.h"
#include "graphics/camera.h"

namespace neko
{
class HelloCameraProgram : public SampleProgram
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
	gl::Shader shader_;
	TextureId  textureWall_;
	Camera3D camera_;
	Mat4f projection_{ Mat4f::Identity };
	EulerAngles cameraAngles{ degree_t(0.0f), degree_t(0.0f), degree_t(0.0f) };

	Vec2f mouseMotion_ = Vec2f::zero;

	const float mouseMotionRatio_ = 50.0f;
	const float cubeDistance_ = 2.0f;
	const int cubeNumbers_ = 10;
	const Vec3f cameraOriginPos = Vec3f(0.0f, 3.0f, -3.0f);
	const EulerAngles cameraOriginAngles = EulerAngles(
		degree_t(45.0f), degree_t(0.0f), degree_t(0.0f));

	enum CameraMovement : std::uint8_t
	{
		NONE = 0u,
		UP = 1u << 0u, 
		DOWN = 1u << 1u, 
		LEFT = 1u << 2u, 
		RIGHT = 1u << 3u,
		DISABLE = 1u << 4u,
		ACCELERATE = 1u << 5u,
		MOUSE_MOVE = 1u << 6u
	};
	const float cameraSpeed_ = 2.0f;
	const float cameraFast_ = 5.0f;
	std::uint8_t cameraMovement_ = NONE;
};
}
