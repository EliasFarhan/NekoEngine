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
#include <map>
#include <string>
#include <mathematics/vector.h>
#include <mathematics/matrix.h>
#include <gl/texture.h>
#include "gl/shader.h"
#include "utils/json_utility.h"

namespace neko::gl
{
using UniformId = std::int32_t;
const UniformId INVALID_UNIFORM_ID = -1;

enum class TextureType
{
    AMBIENT,
    DIFFUSE,
    SPECULAR,
    ALPHA,
    NORMAL,
    ALBEDO,
    METALLIC,
    ROUGHNESS,
    AO,
    LENGTH
};
class Material
{
public:
    explicit Material(gl::TextureManager& textureManager, const FilesystemInterface& filesystem);
    void LoadFromFile(std::string_view path);

    void SetShader(gl::Shader& shader);

    void Bind() const;

    void Destroy();


    void SetBool(std::string_view attributeName, bool value);

    void SetInt(std::string_view attributeName, int value);

    void SetFloat(std::string_view attributeName, float value);

    void SetVec2(std::string_view name, float x, float y);

    void SetVec2(std::string_view name, const Vec2f& value);

    void SetVec3(std::string_view name, float x, float y, float z);

    void SetVec3(std::string_view name, const Vec3f& value);

    void SetVec3(std::string_view name, const float* value);

    void SetVec4(std::string_view name, float x, float y, float z, float w);

    void SetVec4(std::string_view name, const Vec4f& value);

    void SetMat4(std::string_view name, const Mat4f& mat);
    /**
     * \brief Set the texture for the specified type given
     * (should be called before the LoadFromFile function)
     */
    void SetTexture(std::string_view texturePath, TextureType textureType);
    /**
     * \brief Set the texture for the specified type given
     * (should be called before the LoadFromFile function if material has to load the texture)
     */
    void SetTexture(TextureId textureId, TextureType textureType);

    /**
     * \brief Get the uniform location from the uniform name in cache map, otherwise it will get it from OpenGL and
     * put it in the cache
     */
    UniformId GetUniformId(std::string_view uniformName);
protected:
    void FillContent(json& content);
    void LoadTextures();
    void LoadShader();

    const FilesystemInterface& filesystem_;
    gl::TextureManager& textureManager_;
    gl::Shader shader_;
    std::map<std::string, UniformId> uniformsMap_;
    std::array<TextureId, size_t(TextureType::LENGTH)> textureIds_;
    const std::map<TextureType, std::string> textureUniformNames{
            {TextureType::DIFFUSE, "diffuseMap"},
            {TextureType::AMBIENT, "ambientMap"},
            {TextureType::SPECULAR, "specularMap"},
            {TextureType::ALPHA, "alphaMap"},
            {TextureType::NORMAL, "normalMap"},
            {TextureType::ALBEDO, "albedoMap"},
            {TextureType::METALLIC, "metallicMap"},
            {TextureType::ROUGHNESS, "roughnessMap"},
            {TextureType::AO, "aoMap"}
    };
    Job loadingMaterialContentJob_;
    LoadingAssetJob loadMaterialJsonJob_;
};
}