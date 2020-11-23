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

#include "31_hello_pbr_texture/pbr_texture_program.h"

namespace neko
{
void HelloPbrTextureProgram::Init()
{
	textureManager_.Init();
	const auto& config = BasicEngine::GetInstance()->GetConfig();
	sphere_.Init();
	pbrShader_.LoadFromFile(
		config.dataRootPath + "shaders/31_hello_pbr_texture/pbr.vert",
		config.dataRootPath + "shaders/31_hello_pbr_texture/pbr.frag");
	camera_.position = Vec3f::forward * 5.0f;
	camera_.WorldLookAt(Vec3f::zero);

	lights_ = {
		{
		{Vec3f(-10.0f,  10.0f, 10.0f), Vec3f(300.0f, 300.0f, 300.0f)},
		{Vec3f(10.0f,  10.0f, 10.0f),Vec3f(300.0f, 300.0f, 300.0f)},
		{Vec3f(-10.0f, -10.0f, 10.0f),Vec3f(300.0f, 300.0f, 300.0f)},
		{Vec3f(10.0f, -10.0f, 10.0f),Vec3f(300.0f, 300.0f, 300.0f)},
		}
	};
	//Create white texture
	glGenTextures(1, &ao_);
	glBindTexture(GL_TEXTURE_2D, ao_);
	unsigned char white[] = { 255, 255, 255 };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glCheckError();

	albedoId_ = textureManager_.LoadTexture(config.dataRootPath + "textures/rustediron2/rustediron2_basecolor.png", Texture::DEFAULT);
	
	normalId_ = textureManager_.LoadTexture(config.dataRootPath + "textures/rustediron2/rustediron2_normal.png", Texture::DEFAULT);
	
	metallicId_ = textureManager_.LoadTexture(config.dataRootPath + "textures/rustediron2/rustediron2_metallic.png", Texture::DEFAULT);
	
	roughnessId_ = textureManager_.LoadTexture(config.dataRootPath + "textures/rustediron2/rustediron2_roughness.png", Texture::DEFAULT);
	
}

void HelloPbrTextureProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	const auto& config = BasicEngine::GetInstance()->GetConfig();
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);	textureManager_.Update(dt);
}

void HelloPbrTextureProgram::Destroy()
{
	sphere_.Destroy();
	pbrShader_.Destroy();
	textureManager_.Destroy();
	albedo_ = INVALID_TEXTURE_NAME;
}

void HelloPbrTextureProgram::DrawImGui()
{
}

void HelloPbrTextureProgram::Render()
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	if(albedo_ == INVALID_TEXTURE_NAME )
	{
		albedo_ = textureManager_.GetTextureName(albedoId_);
		if (albedo_ == INVALID_TEXTURE_NAME)
			return;
	}
	if (normal_ == INVALID_TEXTURE_NAME)
	{
		normal_ = textureManager_.GetTextureName(normalId_);
		if (normal_ == INVALID_TEXTURE_NAME)
			return;
	}
	if (metallic_ == INVALID_TEXTURE_NAME)
	{
		metallic_ = textureManager_.GetTextureName(metallicId_);
		if (metallic_ == INVALID_TEXTURE_NAME)
			return;
	}
	if (roughness_ == INVALID_TEXTURE_NAME)
	{
		roughness_ = textureManager_.GetTextureName(roughnessId_);
		if (roughness_ == INVALID_TEXTURE_NAME)
			return;
	}
	pbrShader_.Bind();
	pbrShader_.SetTexture("albedoMap", albedo_, 0);
	pbrShader_.SetTexture("normalMap", normal_, 1);
	pbrShader_.SetTexture("metallicMap", metallic_, 2);
	pbrShader_.SetTexture("roughnessMap", roughness_, 3);
	pbrShader_.SetTexture("aoMap", ao_, 4);

	pbrShader_.SetBool("gammaCorrect", gammaCorrect_);

	pbrShader_.SetMat4("view", camera_.GenerateViewMatrix());
	pbrShader_.SetVec3("viewPos", camera_.position);
	pbrShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
	for (size_t i = 0; i < lights_.size(); i++)
	{
		pbrShader_.SetVec3("lights[" + std::to_string(i) + "].position", lights_[i].position);
		pbrShader_.SetVec3("lights[" + std::to_string(i) + "].color", lights_[i].color);

	}
	pbrShader_.SetMat4("model", Mat4f::Identity);
	pbrShader_.SetMat4("normalMatrix", Mat4f::Identity);
	sphere_.Draw();
		
	
}

void HelloPbrTextureProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}
}
