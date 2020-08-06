#pragma once
/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include <mathematics/vector.h>
#include <mathematics/circle.h>

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

    [[nodiscard]] Sphere GenerateBoundingSphere() const
    {
        Sphere s;
        s.center_ = offset_;
        s.radius_ = std::max(std::max(size_.x, size_.y), size_.z);
        return s;
    }

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