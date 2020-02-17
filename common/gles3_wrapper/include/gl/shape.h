#pragma once

#include <mathematics/vector.h>

namespace neko::gl
{
class VertexArrayObject
{
public:
    virtual void Init() = 0;
    virtual void Draw() const = 0;
    virtual void Destroy() = 0;
protected:
    unsigned int VAO = 0;
    unsigned int EBO = 0;
    unsigned int VBO[2]{};
};


class Quad : public VertexArrayObject
{
public:
    explicit Quad(Vec2f origin, Vec2f size);

    void Init() override;

    void Draw() const override;

    void Destroy() override;

protected:
    Vec2f origin_;
    Vec2f size_;
};

class Cuboid : public  VertexArrayObject
{
public:
    explicit Cuboid(Vec3f origin, Vec3f size);
    void Init() override;

    void Draw() const override;

    void Destroy() override;
protected:
    Vec3f origin_;
    Vec3f size_;
};
}