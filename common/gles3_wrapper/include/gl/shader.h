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
#include <string>
#include <graphics/shader.h>
#include "graphics/texture.h"
#include "gl/gles3_include.h"
#include "mathematics/vector.h"
#include "mathematics/matrix.h"

namespace neko::gl
{
class Shader : public neko::Shader
{
public:
    ~Shader() override;

    void LoadFromFile(std::string_view vertexShaderPath, std::string_view fragmentShaderPath) override;


    void Bind() const;

    void Destroy() override;

    GLuint GetProgram();

    void SetBool(std::string_view attributeName, bool value) const;

    void SetInt(std::string_view attributeName, int value) const;

    void SetFloat(std::string_view attributeName, float value) const;

    void SetVec2(std::string_view name, float x, float y) const;

    void SetVec2(std::string_view name, const Vec2f& value) const;

    void SetVec3(std::string_view name, float x, float y, float z) const;

    void SetVec3(std::string_view name, const Vec3f& value) const;

    void SetVec3(std::string_view name, const float* value) const;

    void SetVec4(std::string_view name, float x, float y, float z, float w);

    
    void SetVec4(std::string_view name, const Vec4f& value) const;

    void SetMat4(std::string_view name, const Mat4f& mat) const;

	void SetTexture(std::string_view name, TextureName texture, unsigned int slot = 0) const;
	void SetCubemap(std::string_view name, TextureName texture, unsigned int slot = 0) const;
private:
    GLuint shaderProgram_ = 0;
};
}