/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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
#include "gl/shader.h"
#include <utils/file_utility.h>
#include <sstream>
#include <engine/log.h>
#include <fmt/format.h>
namespace neko::gl
{

void Shader::LoadFromFile(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath)
{
    BufferFile vertexFile = filesystem_.LoadFile(vertexShaderPath);

    GLuint vertexShader = LoadShader(vertexFile, GL_VERTEX_SHADER);
    vertexFile.Destroy();
    if (vertexShader == INVALID_SHADER)
    {
        logDebug(fmt::format("[Error] Loading vertex shader: {} unsuccessful", vertexShaderPath));
        return;
    }
    BufferFile fragmentFile = filesystem_.LoadFile(fragmentShaderPath);

    GLuint fragmentShader = LoadShader(fragmentFile, GL_FRAGMENT_SHADER);
    fragmentFile.Destroy();
    if (fragmentShader == INVALID_SHADER)
    {
        DeleteShader(vertexShader);
        logDebug(fmt::format("[Error] Loading fragment shader: {} unsuccessful", vertexShaderPath));
        return;
    }

    shaderProgram_ = CreateShaderProgram(vertexShader, fragmentShader);
    if(shaderProgram_ == 0)
    {
        logDebug(fmt::format("[Error] Loading shader program with vertex: {} and fragment {}",
                             vertexShaderPath, fragmentShaderPath));
    }
    DeleteShader(vertexShader);
    DeleteShader(fragmentShader);
}

void Shader::Bind() const
{
    glUseProgram(shaderProgram_);
    glCheckError();
}

GLuint Shader::GetProgram() const
{
    return shaderProgram_;
}


void Shader::SetBool(const std::string_view attributeName, bool value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram_, attributeName.data()), (int) value);
    glCheckError();
}

void Shader::SetInt(const std::string_view attributeName, int value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram_, attributeName.data()), value);
    glCheckError();
}

void Shader::SetFloat(const std::string_view attributeName, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgram_, attributeName.data()), value);
    glCheckError();
}

// ------------------------------------------------------------------------
void Shader::SetVec2(const std::string_view name, const Vec2f& value) const
{
    glUniform2fv(glGetUniformLocation(shaderProgram_, name.data()), 1, &value[0]);
    glCheckError();
}

void Shader::SetVec2(const std::string_view name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(shaderProgram_, name.data()), x, y);
    glCheckError();
}

// ------------------------------------------------------------------------
void Shader::SetVec3(const std::string_view name, const Vec3f& value) const
{
    glUniform3fv(glGetUniformLocation(shaderProgram_, name.data()), 1, &value[0]);
    glCheckError();
}

void Shader::SetVec3(const std::string_view name, const float* value) const
{
    glUniform3fv(glGetUniformLocation(shaderProgram_, name.data()), 1, value);
    glCheckError();
}

void Shader::SetVec3(const std::string_view name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(shaderProgram_, name.data()), x, y, z);
    glCheckError();
}

// ------------------------------------------------------------------------
void Shader::SetVec4(const std::string_view name, const Vec4f& value) const
{
    glUniform4fv(glGetUniformLocation(shaderProgram_, name.data()), 1, &value[0]);
    glCheckError();
}

void Shader::SetVec4(const std::string_view name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(shaderProgram_, name.data()), x, y, z, w);
    glCheckError();
}

void Shader::Destroy()
{
    if(shaderProgram_ != 0)
    {
        glDeleteProgram(shaderProgram_);
        shaderProgram_ = 0;
    }
}

/*
// ------------------------------------------------------------------------
void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(shaderProgram_, name.data()), 1, GL_FALSE, &mat[0][0]);
}

// ------------------------------------------------------------------------
void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(shaderProgram_, name.data()), 1, GL_FALSE, &mat[0][0]);
}
*/
// ------------------------------------------------------------------------
void Shader::SetMat4(const std::string_view name, const Mat4f& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram_, name.data()), 1, GL_FALSE, &mat[0][0]);
    glCheckError();
}


void Shader::SetTexture(const std::string_view name, TextureName texture, unsigned slot) const
{
    glUniform1i(glGetUniformLocation(shaderProgram_, name.data()), slot);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);
}


void Shader::SetCubemap(const std::string_view name, TextureName texture, unsigned slot) const
{
    glUniform1i(glGetUniformLocation(shaderProgram_, name.data()), slot);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

Shader::~Shader()
{
    Destroy();
}

Shader::Shader() : filesystem_(BasicEngine::GetInstance()->GetFilesystem())
{

}

GLuint LoadShader(const BufferFile& shaderfile, GLenum shaderType)
{
    if(shaderfile.dataBuffer == nullptr)
        return INVALID_SHADER;
    return LoadShader(reinterpret_cast<char*>(shaderfile.dataBuffer), shaderType);
}

GLuint CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader, GLuint computeShader, GLuint geometryShader,
                           GLuint tesselationControlShader, GLuint tesselationEvaluationShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    if(computeShader != INVALID_SHADER)
    {
        glAttachShader(program, computeShader);
    }
    if(geometryShader != INVALID_SHADER)
    {
        glAttachShader(program, geometryShader);
    }
    if(tesselationControlShader != INVALID_SHADER)
    {
        glAttachShader(program, tesselationControlShader);
    }
    if(tesselationEvaluationShader != INVALID_SHADER)
    {
        glAttachShader(program, tesselationEvaluationShader);
    }
    glLinkProgram(program);
    //Check if shader program was linked correctly
    GLint success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        logDebug(fmt::format("[Error] Shader program with vertex {} and fragment {}: LINK_FAILED with infoLog:\n{}",
                             vertexShader,
                             fragmentShader,
                             infoLog));
        return 0;
    }
    return program;
}

GLuint gl::LoadShader(char* shaderContent, GLenum shaderType)
{
    glCheckError();
    const GLuint shader = glCreateShader(shaderType);
    glCheckError();


    glShaderSource(shader, 1, &shaderContent, nullptr);
    glCompileShader(shader);
    //Check success status of shader compilation
    GLint success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        logDebug(fmt::format("[Error] Shader compilation failed with this log:\n{}\nShader content:\n{}",
                             infoLog,
                             shaderContent));
        return 0;
    }
    return shader;
}

void DeleteShader(GLuint shader)
{
    glDeleteShader(shader);
}
}
