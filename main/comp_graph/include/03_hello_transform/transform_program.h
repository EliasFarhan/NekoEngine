#pragma once

#include <comp_graph/sample_program.h>
#include "gl/shape.h"
#include "gl/shader.h"
#include "mathematics/vector.h"

namespace neko
{

class HelloTransformProgram : public SampleProgram
{
public:
    void Init() override;
    void Update(seconds) override;
    void Render() override;
    void Destroy() override;

    void DrawUi(seconds dt) override;

    void OnEvent(const SDL_Event& event) override;

private:

    gl::Quad quad{Vec2f::Zero, Vec2f::One};
    Mat4f transform;
    gl::Shader shaderProgram;
    unsigned VBO[2];
    unsigned EBO;
    unsigned VAO;
    gl::TextureId textureWall;
};
}