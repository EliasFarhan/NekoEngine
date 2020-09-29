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

namespace neko::gl
{

void Shader::LoadFromFile(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath)
{
    if(!FileExists(vertexShaderPath))
    {
        logDebug(std::string("[Error] Vertex shader: ")+vertexShaderPath.data()+" does not exist");
    }
    glCheckError();
    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glCheckError();

    const auto vertexShaderProgram = LoadFile(vertexShaderPath.data());
    const char* vertexShaderChar = vertexShaderProgram.c_str();

    glShaderSource(vertexShader, 1, &vertexShaderChar, NULL);
    glCompileShader(vertexShader);
    //Check success status of shader compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::ostringstream oss;
        oss << "[Error] Vertex shader at: " << vertexShaderPath <<" compilation failed: \n" << infoLog<<'\n'<<vertexShaderChar;
        logDebug(oss.str());
        return;
    }
    glCheckError();
    if(!FileExists(fragmentShaderPath))
    {
        logDebug(std::string("[Error] Fragment shader: ")+fragmentShaderPath.data()+" does not exist");
    }
    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glCheckError();
    auto fragmentShaderProgram = LoadFile(fragmentShaderPath.data());
    const char* fragmentShaderChar = fragmentShaderProgram.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderChar, NULL);
    glCompileShader(fragmentShader);
    //Check success status of shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::ostringstream oss;
        oss << "[Error] Fragment shader at: " << fragmentShaderPath <<" compilation failed\n" << infoLog << '\n' << fragmentShaderChar;
        logDebug(oss.str());
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
        glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
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


void Shader::SetBool(const std::string& attributeName, bool value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram_, attributeName.c_str()), (int) value);
}

void Shader::SetInt(const std::string& attributeName, int value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram_, attributeName.c_str()), value);
}

void Shader::SetFloat(const std::string& attributeName, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgram_, attributeName.c_str()), value);
}

// ------------------------------------------------------------------------
void Shader::SetVec2(const std::string& name, const Vec2f& value) const
{
    glUniform2fv(glGetUniformLocation(shaderProgram_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(shaderProgram_, name.c_str()), x, y);
}

// ------------------------------------------------------------------------
void Shader::SetVec3(const std::string& name, const Vec3f& value) const
{
    glUniform3fv(glGetUniformLocation(shaderProgram_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, const float value[3]) const
{
    glUniform3fv(glGetUniformLocation(shaderProgram_, name.c_str()), 1, value);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(shaderProgram_, name.c_str()), x, y, z);
}

// ------------------------------------------------------------------------
void Shader::SetVec4(const std::string& name, const Vec4f& value) const
{
    glUniform4fv(glGetUniformLocation(shaderProgram_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(shaderProgram_, name.c_str()), x, y, z, w);
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
    glUniformMatrix2fv(glGetUniformLocation(shaderProgram_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// ------------------------------------------------------------------------
void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(shaderProgram_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
*/
// ------------------------------------------------------------------------
void Shader::SetMat4(const std::string& name, const Mat4f& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::SetTexture(const std::string& name, TextureName texture, unsigned slot) const
{
    glUniform1i(glGetUniformLocation(shaderProgram_, name.c_str()), slot);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);
}


void Shader::SetCubemap(const std::string& name, TextureName texture, unsigned slot) const
{
    glUniform1i(glGetUniformLocation(shaderProgram_, name.c_str()), slot);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

Shader::~Shader()
{
    Destroy();
}
}
