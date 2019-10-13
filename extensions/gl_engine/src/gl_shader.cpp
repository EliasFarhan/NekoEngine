#include "gl_engine/gl_shader.h"
#include <utilities/file_utility.h>
#include <sstream>
#include <engine/log.h>

namespace neko::gl
{

void Shader::CompileSource(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath)
{
    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
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
        oss << "[Error] Vertex shader at: " << vertexShaderPath <<" compilation failed: \n" << infoLog;
        logDebug(oss.str());
        return;
    }

    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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
        oss << "[Error] Fragment shader at: " << fragmentShaderPath <<" compilation failed\n" << infoLog;
        logDebug(oss.str());
        return;
    }

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

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}




void Shader::Bind()
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
void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(shaderProgram_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(shaderProgram_, name.c_str()), x, y);
}

// ------------------------------------------------------------------------
void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
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
void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(shaderProgram_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(shaderProgram_, name.c_str()), x, y, z, w);
}

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

// ------------------------------------------------------------------------
void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

}