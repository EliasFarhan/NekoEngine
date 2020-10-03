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

    void LoadFromFile(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath) override;


    void Bind() const;

    void Destroy() override;

    GLuint GetProgram();

    void SetBool(const std::string& attributeName, bool value) const;

    void SetInt(const std::string& attributeName, int value) const;

    void SetFloat(const std::string& attributeName, float value) const;

    void SetVec2(const std::string& name, float x, float y) const;

    void SetVec2(const std::string& name, const Vec2f& value) const;

    void SetVec3(const std::string& name, float x, float y, float z) const;

    void SetVec3(const std::string& name, const Vec3f& value) const;

    void SetVec3(const std::string& name, const float value[3]) const;

    void SetVec4(const std::string& name, float x, float y, float z, float w);

    
    void SetVec4(const std::string& name, const Vec4f& value) const;
/*
    void SetMat2(const std::string& name, const glm::mat2& mat) const;

    void SetMat3(const std::string& name, const glm::mat3& mat) const;
*/
    void SetMat4(const std::string& name, const Mat4f& mat) const;

	void SetTexture(const std::string& name, TextureName texture, unsigned int slot = 0) const;
	void SetCubemap(const std::string& name, TextureName texture, unsigned int slot = 0) const;
private:
    GLuint shaderProgram_ = 0;
};
}