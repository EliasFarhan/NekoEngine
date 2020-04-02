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
    struct Circle2D
    {
        explicit Circle2D(Vec2f Center, float Radius) : center(Center), radius(Radius)
        {

        }

    	float GetRadius() const
        {
            return radius;
        }

    	bool Intersects(Circle2D circle) const
        {
            const Vec2f distanceVector = circle.center - center;
        	
            return distanceVector.Magnitude() <= circle.radius + radius;
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

    	Vec2f center;
        const float radius = 0;
    };
    struct Plan3D
    {
        explicit Plan3D(Vec3f Pos, Vec3f Normal) : pos(Pos), normal(Normal)
        {
            normal = normal.Normalized();
        }

        Vec3f pos;
        Vec3f normal;
    };
    //TODO add Sphere3D structure
    struct Sphere3D
    {
        explicit Sphere3D(Vec3f Center, float Radius) : center(Center), radius(Radius)
        {

        }

        float GetRadius() const
        {
            return radius;
        }
		
        bool Intersects(Sphere3D sphere) const
        {
            const Vec3f distanceVector = sphere.center - center;

            return distanceVector.Magnitude() <= sphere.radius + radius;
        }
		
        bool IsPlanCircleIntersects(Plan3D plan) const
        {
            const float p = Vec3f::Dot(center - plan.pos, plan.normal) / plan.normal.Magnitude();

            return p < radius && p > -radius;
        }
		
        Vec3f center;
        float radius;
    };
    
    using Circle = Circle2D;
    using Sphere = Sphere3D;
    using Plan = Plan3D;
	
}