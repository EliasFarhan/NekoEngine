#pragma once


#include "comp_graph/sample_program.h"
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "mathematics/vector.h"

namespace neko
{

class HelloCubeProgram : public SampleProgram
{
public:
    void Init() override;
    void Update(seconds) override;
    void Render() override;
    void Destroy() override;

    void DrawUi(seconds dt) override;

    void OnEvent(const SDL_Event& event) override;

private:

    gl::Cuboid cube_{ Vec3f::Zero, Vec3f::One };
    Mat4f transform_{ Mat4f::Identity };
    Vec3f position_ = Vec3f::Zero;
    Vec3f scale_ = Vec3f::One;
    Vec3f eulerAngle_{Vec3f::Zero};
    gl::Shader shaderProgram_;
    gl::TextureId textureWall_;
};
}