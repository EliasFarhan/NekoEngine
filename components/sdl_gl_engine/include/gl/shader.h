#pragma once
#include <string>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>

namespace neko::gl
{
class Shader
{
public:
    void CompileSource(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath);


    void Bind();

    GLuint GetProgram();

    void SetBool(const std::string& attributeName, bool value) const;

    void SetInt(const std::string& attributeName, int value) const;

    void SetFloat(const std::string& attributeName, float value) const;

    void SetVec2(const std::string& name, float x, float y) const;

    void SetVec2(const std::string& name, const glm::vec2& value) const;

    void SetVec3(const std::string& name, float x, float y, float z) const;

    void SetVec3(const std::string& name, const glm::vec3& value) const;

    void SetVec3(const std::string& name, const float value[3]) const;

    void SetVec4(const std::string& name, float x, float y, float z, float w);

    void SetVec4(const std::string& name, const glm::vec4& value) const;

    void SetMat2(const std::string& name, const glm::mat2& mat) const;

    void SetMat3(const std::string& name, const glm::mat3& mat) const;

    void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
    GLuint shaderProgram_ = 0;
};
}