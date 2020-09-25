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
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "gl/model.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{

class HelloCubemapsProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	enum class ModelRenderMode
	{
		NONE,
		REFLECTION,
		REFRACTION,
		LENGTH
	};
	gl::RenderCuboid skyboxCube_{ Vec3f::zero, Vec3f::one*2.0f };
	gl::Shader skyboxShader_;
	
	TextureName skyboxTexture_ = 0;

	gl::Shader modelShader_;
	gl::Shader modelReflectionShader_;
	gl::Shader modelRefractionShader_;
	assimp::Model model_;
	gl::RenderCuboid cube_{Vec3f::zero, Vec3f::one};
	gl::TextureManager textureManager_;
	TextureName cubeTexture_ = INVALID_TEXTURE_NAME;
	TextureId cubeTextureId_;
	sdl::Camera3D camera_;
	
	ModelRenderMode currentRenderMode_ = ModelRenderMode::NONE;
	float reflectionValue_ = 1.0f;
	float refractionValue_ = 1.0f;
	float refractiveIndex_ = 1.52f;

	
	
};

}
