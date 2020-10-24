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
#include <utilities/file_utility.h>
#include <sstream>
#include <engine/log.h>
#include <fmt/format.h>
namespace neko::gl
{

void Shader::LoadFromFile(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath)
{
    if(!FileExists(vertexShaderPath))
    {
        logDebug(fmt::format("[Error] Vertex shader: {}  does not exist", vertexShaderPath));
    }
    glCheckError();
    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glCheckError();

    const auto vertexShaderProgram = LoadFile(vertexShaderPath.data());
    const char* vertexShaderContent = vertexShaderProgram.data();

    glShaderSource(vertexShader, 1, &vertexShaderContent, nullptr);
    glCompileShader(vertexShader);
    //Check success status of shader compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        logDebug(fmt::format("[Error] Vertex shader at: {} compilation failed with this log:\n{}\nShader content:\n{}", 
            vertexShaderPath, 
            infoLog, 
            vertexShaderContent));
        return;
    }
    glCheckError();
    if(!FileExists(fragmentShaderPath))
    {
        logDebug(std::string("[Error] Fragment shader: ")+fragmentShaderPath.data()+" does not exist");
    }
    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glCheckError();
    const auto fragmentShaderProgram = LoadFile(fragmentShaderPath.data());
    const char* fragmentShaderContent = fragmentShaderProgram.data();
    glShaderSource(fragmentShader, 1, &fragmentShaderContent, nullptr);
    glCompileShader(fragmentShader);
    //Check success status of shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        logDebug(fmt::format("[Error] Fragment shader at: {}  compilation failed with log:\n{}\nShader content:\n{}",
            fragmentShaderPath, infoLog, fragmentShaderContent));
        return;
    }
    glCheckError();
    shaderProgram_ = glCreateProgram();
    glAttachShader(shaderProgram_, vertexShader);
    glAttachShader(shaderProgram_, fragmentShader);
    glLinkProgram(shaderProgram_);
    //Check if shader program was linked correctly
    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram_, 512, nullptr, infoLog);
        std::ostringstream oss;
        oss << "[Error] Shader program with vertex: "<< vertexShaderPath<<" and fragment:"<< fragmentShaderPath << "LINK_FAILED\n" << infoLog;
        return;
    }
    glCheckError();
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glCheckError();
}

void Shader::Bind() const
{
    glUseProgram(shaderProgram_);
}

GLuint Shader::GetProgram()
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
}
