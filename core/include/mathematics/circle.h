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
    template<class T = Vec2f>
    struct Circle
    {
        explicit Circle(T Center, float Radius) : center(Center), radius(Radius)
        {

        }

    	float GetRadius() const
        {
            return radius;
        }

    	bool Intersects(Circle<T> circle) const
        {
            const T distanceVector = circle.center - center;
        	
            return distanceVector.Magnitude() <= circle.radius + radius;
        }

        bool RectCircleIntersects(Rect2f rect) const
        {
			const T distanceVector = rect.center - center;

			if (distanceVector.Magnitude() <= rect.halfSize.x + radius)
			{
                return true;
			}

			if (distanceVector.Magnitude() <= rect.halfSize.y + radius)
			{
                return true;
			}
        	
            return false;
        }

    	static bool IsPlanCircleIntersects(const Circle<Vec3f> sphere, const Vec3f normal, const Vec3f pos)
        {
            const float p = Vec3f::Dot(sphere.center - pos, normal) / normal.Magnitude();
        	
            return p < sphere.radius && p > -sphere.radius;
        }
    	
    	T center;
        const float radius = 0;
    };

    using Circle2D = Circle<Vec2f>;
    using Sphere3D = Circle<Vec3f>;
}