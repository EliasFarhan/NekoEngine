#pragma once

#include <comp_graph/sample_program.h>
#include "gl/shape.h"
#include "gl/shader.h"
#include <gl/texture.h>
// #include "mathematics/vector.h" // Included in transform.h
#include "mathematics/transform.h"

namespace neko
{

class HelloTransformProgram : public SampleProgram
{
public:
    void Init() override;
    void Update(seconds) override;
    void Render() override;
    void Destroy() override;

    void DrawUi() override;

    void OnEvent(const SDL_Event& event) override;

private:

    enum class ShapeType
    {
        PLANE = 0,
        CUBE,
        LENGTH
    };
    ShapeType shape_ = ShapeType::PLANE;
    gl::Cuboid cube_{ Vec3f::zero, Vec3f::one };
    gl::Quad quad_{Vec2f::zero, Vec2f::one};
    Mat4f transform_{Mat4f::Identity};
    Vec3f position_ = Vec3f::zero;
    Vec3f scale_ = Vec3f::one;
    float angle_ = 0.0f;
    EulerAngles eulerAngle_{Vec3f::zero};
    gl::Shader shaderProgram_;
    gl::TextureId textureWall_;
};
}