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
#include "gl/texture.h"
#include "gl/shape.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloBlinnProgram : public SampleProgram
{
public:
    virtual ~HelloBlinnProgram()= default;
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	gl::Shader modelShader_;
	gl::Shader blinnShader_;

	assimp::Model model_;
	gl::RenderQuad floor_{Vec3f::zero, Vec2f::one*10.0f};
	gl::TextureManager textureManager_;
	TextureName floorTexture_ = INVALID_TEXTURE_NAME;
	TextureId floorTextureId_;
	float floorResolution_ = 6.0f;
	int specularPow_ = 32;
	enum BlinnFlags : std::uint8_t
	{
		NONE = 0u,
		ENABLE_BLINN = 1u << 0u,
		ONLY_SPECULAR = 1u << 1u
	};
	sdl::Camera3D camera_;
	Vec3f lightPos_;
	float dt_ = 0.0f;
	std::uint8_t flags_ = NONE;
};
}
