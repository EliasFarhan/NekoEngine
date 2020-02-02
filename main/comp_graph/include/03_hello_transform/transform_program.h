#pragma once

#include <comp_graph/sample_program.h>
#include "gl/shape.h"
#include "gl/shader.h"

namespace neko
{

class HelloTransformDrawingProgram : public SampleProgram
{
public:
    void Init() override;
    void Update(seconds) override;
    void Render() override;
    void Destroy() override;
private:

    gl::Quad quad;
    Mat4f transform;
    gl::Shader shaderProgram;
    unsigned VBO[2];
    unsigned EBO;
    unsigned VAO;
    unsigned textureWall;
};
}