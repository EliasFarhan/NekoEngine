#pragma once

#include <mathematics/vector.h>

namespace neko
{

class RenderableObject
{
public:
    virtual ~RenderableObject() = default;
    virtual void Init() = 0;
    virtual void Draw() const = 0;
    virtual void Destroy() = 0;
};

class RenderShape : RenderableObject
{
public:
    RenderShape() = delete;
    explicit RenderShape(Vec3f offset) : offset_(offset){};
protected:
    Vec3f offset_;
};

class RenderCircle : public RenderShape
{
public:
    RenderCircle() = delete;
    explicit RenderCircle(Vec3f offset, float radius) : RenderShape(offset), radius_(radius){}
protected:
    float radius_ = 0.0f;
    static const size_t resolution = 50;
};

class RenderQuad : public RenderShape
{
public:
    RenderQuad() = delete;
    explicit RenderQuad(Vec3f offset, Vec2f size) : RenderShape(offset), size_(size){}
protected:
    Vec2f size_;
};

class RenderCuboid : public RenderShape
{
public:
    RenderCuboid()=delete;
    explicit RenderCuboid(Vec3f offset, Vec3f size) : RenderShape(offset), size_(size){}
protected:
    Vec3f size_;
};

class RenderSphere : public RenderShape
{
public:
    RenderSphere() = delete;
    explicit RenderSphere(Vec3f offset, float radius) : RenderShape(offset), radius_(radius){}
protected:
    float radius_ = 0.0f;
};
}