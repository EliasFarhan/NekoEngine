#pragma once

#include <comp_graph/sample_program.h>
#include "gl/shape.h"
#include "gl/shader.h"
#include <gl/texture.h>
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

    enum class ShapeType
    {
        PLANE = 0,
        CUBE,
        LENGTH
    };
    ShapeType shape_ = ShapeType::PLANE;
    gl::Cuboid cube_{ Vec3f::Zero, Vec3f::One };
    gl::Quad quad_{Vec2f::Zero, Vec2f::One};
    Mat4f transform_{Mat4f::Identity};
    Vec3f position_ = Vec3f::Zero;
    Vec3f scale_ = Vec3f::One;
    float angle_ = 0.0f;
    EulerAngles eulerAngle_{Vec3f::Zero};
    gl::Shader shaderProgram_;
    gl::TextureId textureWall_;
};
}