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

#include "gl/material.h"

namespace neko::gl
{

Material::Material(TextureManager& textureManager, const FilesystemInterface& filesystem) :
        filesystem_(filesystem),
        textureManager_(textureManager),
        loadingMaterialContentJob_([this]()
        {
            json matJson = json::parse(loadMaterialJsonJob_.GetBufferFile().dataBuffer);
            FillContent(matJson);
            LoadShader();
            LoadTextures();
        }),
        loadMaterialJsonJob_(filesystem)
{

}

void Material::LoadFromFile(std::string_view path)
{
    BasicEngine::GetInstance()->ScheduleJob(&loadMaterialJsonJob_, JobThreadType::RESOURCE_THREAD);
    loadingMaterialContentJob_.AddDependency(&loadMaterialJsonJob_);
    BasicEngine::GetInstance()->ScheduleJob(&loadingMaterialContentJob_, JobThreadType::OTHER_THREAD);
}

void Material::Bind() const
{

}

void Material::Destroy()
{

}

void Material::SetShader(Shader& shader)
{

}

void Material::SetBool(std::string_view attributeName, bool value)
{
    auto uniformId = GetUniformId(attributeName);
    if (uniformId != INVALID_UNIFORM_ID)
    {
        glUniform1i(uniformId, value);
    }
}

void Material::SetInt(std::string_view attributeName, int value)
{
    auto uniformId = GetUniformId(attributeName);
    if (uniformId != INVALID_UNIFORM_ID)
    {
        glUniform1i(uniformId, value);
    }
}

void Material::SetFloat(std::string_view attributeName, float value)
{
    auto uniformId = GetUniformId(attributeName);
    if (uniformId != INVALID_UNIFORM_ID)
    {
        glUniform1f(uniformId, value);
    }
}

void Material::SetVec2(std::string_view name, float x, float y)
{
    auto uniformId = GetUniformId(name);
    if (uniformId != INVALID_UNIFORM_ID)
    {
        glUniform2f(uniformId, x, y);
    }
}

void Material::SetVec2(std::string_view name, const Vec2f& value)
{
    auto uniformId = GetUniformId(name);
    if (uniformId != INVALID_UNIFORM_ID)
    {
        glUniform2fv(uniformId, 2, &value[0]);
    }
}

void Material::SetVec3(std::string_view name, float x, float y, float z)
{
    auto uniformId = GetUniformId(name);
    if (uniformId != INVALID_UNIFORM_ID)
    {
        glUniform3f(uniformId, x, y, z);
    }

}

void Material::SetVec3(std::string_view name, const Vec3f& value)
{
    auto uniformId = GetUniformId(name);
    if (uniformId != INVALID_UNIFORM_ID)
    {
        glUniform3fv(uniformId, 1, &value[0]);
    }
}

void Material::SetVec3(std::string_view name, const float* value)
{
    auto uniformId = GetUniformId(name);
    if (uniformId != INVALID_UNIFORM_ID)
    {
        glUniform3fv(uniformId, 1, value);
    }
}

void Material::SetVec4(std::string_view name, float x, float y, float z, float w)
{
    auto uniformId = GetUniformId(name);
    if (uniformId != INVALID_UNIFORM_ID)
    {
        glUniform4f(uniformId, x, y, z, w);
    }

}

void Material::SetVec4(std::string_view name, const Vec4f& value)
{
    auto uniformId = GetUniformId(name);
    if (uniformId != INVALID_UNIFORM_ID)
    {
        glUniform4fv(uniformId, 1, &value[0]);
    }

}

void Material::SetMat4(std::string_view name, const Mat4f& mat)
{
    auto uniformId = GetUniformId(name);
    if (uniformId != INVALID_UNIFORM_ID)
    {
        glUniformMatrix4fv(uniformId, 1, GL_FALSE, &mat[0][0]);
    }

}

UniformId Material::GetUniformId(std::string_view uniformName)
{
    auto it = uniformsMap_.find(uniformName.data());
    if (it != uniformsMap_.end())
    {
        return it->second;
    }

    UniformId uniformId = glGetUniformLocation(shader_.GetProgram(), uniformName.data());
    if (uniformId != INVALID_UNIFORM_ID)
    {
        uniformsMap_[uniformName.data()] = uniformId;
    }
    return uniformId;
}

void Material::LoadShader()
{
    //TODO load shader

    //Load uniform cache
    for(auto& it : uniformsMap_)
    {
        GetUniformId(it.first);
    }

}

void Material::LoadTextures()
{

}

void Material::FillContent(json& content)
{
    if(CheckJsonExists(content, "uniforms"))
    {
        for(auto& uniform : content["uniforms"])
        {
            uniformsMap_[uniform["name"]] = INVALID_UNIFORM_ID;
        }
    }
}

}