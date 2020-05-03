#pragma once

#include <comp_graph/sample_program.h>
#include "gl/shape.h"
#include "gl/shader.h"
#include <gl/texture.h>
#include "mathematics/matrix.h"

namespace neko
{

class HelloTransformProgram : public SampleProgram
{
public:
    void Init() override;
    void Update(seconds) override;
    void Render() override;
    void Destroy() override;

    void DrawImGui() override;

    void OnEvent(const SDL_Event& event) override;

private:

    enum class ShapeType
    {
        PLANE = 0,
        CUBE,
        LENGTH
    };
    ShapeType shape_ = ShapeType::PLANE;
    gl::RenderCuboid cube_{ Vec3f::zero, Vec3f::one };
    gl::RenderQuad quad_{Vec3f::zero, Vec2f::one};
    Mat4f transform_{Mat4f::Identity};
    Vec3f position_ = Vec3f::zero;
    Vec3f scale_ = Vec3f::one;
    degree_t angle_{0.0f};
    EulerAngles eulerAngle_{Vec3f::zero};
    gl::Shader shaderProgram_;
    gl::TextureId textureWall_;
};
}