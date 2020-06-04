#pragma once
#include "comp_graph/sample_program.h"
#include "gl/model.h"
#include "gl/shader.h"
#include "gl/shape.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloFrustumProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	void CalculateForce(size_t begin, size_t end);
	void CalculateVelocity(size_t begin, size_t end);
	void CalculatePositions(size_t begin, size_t end);
	void Culling(size_t begin, size_t end);


	sdl::Camera3D camera_;
	sdl::Camera3D overCamera_;
	assimp::Model model_;

	const size_t maxAsteroidNmb_ = 100'000;
	const size_t minAsteroidNmb_ = 1'000;
	size_t instanceChunkSize_ = 1'000;
	size_t asteroidNmb_ = 1000;

	size_t culledAsteroids_ = 0;

	gl::Shader vertexInstancingDrawShader_;
	gl::Shader screenShader_;

	std::vector<Vec3f> asteroidPositions_;
	std::vector<Vec3f> asteroidVelocities_;
	std::vector<Vec3f> asteroidForces_;
	/**
	 * Used by frustum culling before sending to GPU
	 */
	std::vector<Vec3f> asteroidCulledPositions_;


	unsigned int instanceVBO_ = 0;

	const float gravityConst = 1000.0f;
	const float centerMass = 1000.0f;
	const float asteroidMass = 1.0f;
	float dt_ = 1.0f;

	unsigned int fbo_ = 0;
	unsigned int overViewTexture_ = 0;
	unsigned int rbo_ = 0;

	gl::RenderQuad mainPlane_{ Vec3f::zero, Vec2f::one * 2.0f };
};


}
