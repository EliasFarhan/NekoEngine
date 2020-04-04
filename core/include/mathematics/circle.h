#pragma once
/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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
#include "mathematics/vector.h"
#include "rect.h"

namespace neko
{
    struct Circle
    {
        Vec2f center;
        const float radius;

        explicit Circle(Vec2f Center, const float Radius) : center(Center), radius(Radius)
        {

        }

        bool Intersects(Circle circle) const
        {
            const float distanceX = center.x - circle.center.x;
            const float distanceY = center.y - circle.center.y;
            const float radiusSum = circle.radius + radius;

            return distanceX * distanceX + distanceY * distanceY <= radiusSum * radiusSum;
        }

        bool IntersectsOther(Circle circle) const
        {
            const Vec2f distanceVector = circle.center - center;
            const float magnitude = Vec2f::Dot(distanceVector, distanceVector) * 2;

            return magnitude <= circle.radius + radius;
        }

        bool RectCircleIntersects(Rect2f rect) const
        {
            const Vec2f distanceVector = rect.center - center;
            const float magnitude = distanceVector.Magnitude();

            if (magnitude <= rect.halfSize.x + radius)
            {
                return true;
            }

            if (magnitude <= rect.halfSize.y + radius)
            {
                return true;
            }

            return false;
        }
    };

    struct Plan
    {
        Vec3f pos;
        Vec3f normal;

        explicit Plan(Vec3f Pos, Vec3f Normal) : pos(Pos), normal(Normal)
        {

        }
    };

    struct Sphere
    {
        Vec3f center;
        const float radius;
    	
        explicit Sphere(Vec3f Center, float Radius) : center(Center), radius(Radius)
        {

        }

        bool Intersects(Sphere sphere) const
        {
            const float distanceX = center.x - sphere.center.x;
            const float distanceY = center.y - sphere.center.y;
            const float distanceZ = center.z - sphere.center.z;

            const float radiusSum = sphere.radius + radius;

            return distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ <= radiusSum * radiusSum;
        }

        bool IntersectsOther(Sphere sphere) const
        {
            const Vec3f distanceVector = sphere.center - center;
            const float magnitude = distanceVector.Magnitude();
            return magnitude <= sphere.radius + radius;
        }

        bool IsPlanSphereIntersects(Plan plan) const
        {
            const float normMagnitude = plan.normal.Magnitude();
            const float dot = Vec3f::Dot(center - plan.pos, plan.normal);

            const float p = dot / normMagnitude;

            return p < radius&& p > -radius;
        }
    };
}