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
#include "gl/shader.h"
#include "gl/model.h"
#include "gl/shape.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{

class HelloCullingProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	enum CullingFlags : std::uint8_t
	{
		NONE,
		CULLING = 1u << 0u,
		BACK_CULLING = 1u << 1u,
		CCW = 1u << 2u
	};
	
	sdl::Camera3D camera_;
    gl::ModelId modelId_ = gl::INVALID_MODEL_ID;
    gl::ModelManager modelManager_;
	gl::RenderCuboid cube_{ Vec3f::zero, Vec3f::one };
	gl::TextureManager textureManager_;
	gl::TextureName cubeTexture_ = gl::INVALID_TEXTURE_NAME;
	gl::TextureId cubeTextureId_;
	gl::Shader modelShader_;

	sdl::Camera3D externalCamera_;

	std::uint8_t flags_ = BACK_CULLING | CCW;
};

}
