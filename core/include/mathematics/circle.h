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

    	const bool CirclesIntersect(Circle<T> circle) const
        {
            const T distanceVector = circle.center - center;
        	
            if (Distance(distanceVector.GetMagnitude()) <= circle.radius + radius)
            {
                return true;            	
            }
        	
            return false;
        }

        const bool SquareCircleIntersect(neko::Rect2f rect) const
        {
			const T distanceVector = rect.center - center;

			if (Distance(distanceVector.GetMagnitude()) <= rect.halfSize.x + radius)
			{
                return true;
			}

			if (Distance(distanceVector.GetMagnitude()) <= rect.halfSize.y + radius)
			{
                return true;
			}
            return false;
        }

    	static bool IsPlanCircleContact(Circle<Vec3f> sphere, Vec3f normal, Vec3f pos)
        {
            const float p = Vec3f::Dot(sphere.center - pos, normal) / normal.GetMagnitude();
        	
            return p < sphere.radius && p > -sphere.radius;
        }
    	
        static float Distance(const float magnitude)
        {
            return magnitude;
        }
    	
    	T center;
        const float radius = 0;
    };

    using Circle2D = Circle<Vec2f>;
    using Sphere3D = Circle<Vec3f>;
}