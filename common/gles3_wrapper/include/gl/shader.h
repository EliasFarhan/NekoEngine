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
const GLuint INVALID_SHADER = 0;
/**
 * Load shader with given shader type
 * (GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER,
 * GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER or GL_FRAGMENT_SHADER)
 */
GLuint LoadShader(const BufferFile& shaderfile, GLenum shaderType);
GLuint LoadShader(char* shaderContent, GLenum shaderType);
/**
 * Link shaders together in a program
 */
GLuint CreateShaderProgram(
        GLuint vertexShader,
        GLuint fragmentShader,
        GLuint computeShader = INVALID_SHADER,
        GLuint geometryShader = INVALID_SHADER,
        GLuint tesselationControlShader = INVALID_SHADER,
        GLuint tesselationEvaluationShader = INVALID_SHADER);

void DeleteShader(GLuint shader);

class Shader : public neko::Shader
{
public:
    explicit Shader();
    ~Shader() override;

    void LoadFromFile(std::string_view vertexShaderPath, std::string_view fragmentShaderPath) override;


    void Bind() const;

    void Destroy() override;

    GLuint GetProgram() const;

    void SetBool(const std::string_view attributeName, bool value) const;

    void SetInt(const std::string_view attributeName, int value) const;

    void SetFloat(const std::string_view attributeName, float value) const;

    void SetVec2(const std::string_view name, float x, float y) const;

    void SetVec2(const std::string_view name, const Vec2f& value) const;

    void SetVec3(const std::string_view name, float x, float y, float z) const;

    void SetVec3(const std::string_view name, const Vec3f& value) const;

    void SetVec3(const std::string_view name, const float* value) const;

    void SetVec4(const std::string_view name, float x, float y, float z, float w);

    
    void SetVec4(const std::string_view name, const Vec4f& value) const;

    void SetMat4(const std::string_view name, const Mat4f& mat) const;

	void SetTexture(const std::string_view name, TextureName texture, unsigned int slot = 0) const;
	void SetCubemap(const std::string_view name, TextureName texture, unsigned int slot = 0) const;
private:
    const FilesystemInterface& filesystem_;
    GLuint shaderProgram_ = 0;
};
}