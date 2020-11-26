#pragma once
/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
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
	gl::ModelId modelId_ = gl::INVALID_MODEL_ID;
	gl::ModelManager modelManager_;

	gl::TextureManager textureManager_;

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
