#pragma once

namespace neko
{
class Shader
{
public:
    virtual void LoadFromFile(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath) = 0;

protected:
};
}