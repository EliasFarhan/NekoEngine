#pragma once

#include <mathematics/vector.h>
#include <graphics/shape.h>

namespace neko::gl
{
struct VertexArrayObject
{
    unsigned int VAO = 0;
    unsigned int EBO = 0;
    unsigned int VBO[4]{};
};

class RenderCircle : public neko::RenderCircle, public VertexArrayObject
{
public:
    using neko::RenderCircle::RenderCircle;
    void Init() override;

    void Draw() const override;

    void Destroy() override;
};

class RenderQuad : public neko::RenderQuad, public VertexArrayObject
{
public:
    using neko::RenderQuad::RenderQuad;
    void Init() override;

    void Draw() const override;

    void Destroy() override;
};

class RenderCuboid : public neko::RenderCuboid, public VertexArrayObject
{
public:
    using neko::RenderCuboid::RenderCuboid;
    void Init() override;

    void Draw() const override;

    void Destroy() override;
};

class RenderSphere : public neko::RenderSphere, public VertexArrayObject
{
public:
	explicit RenderSphere(Vec3f offset, float radius, size_t segment = 64);

    void Init() override;

    void Draw() const override;

    void Destroy() override;
protected:
    size_t segment_ = 0;
	size_t indexCount_ = 0;
};
}