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
#include <array>

#include <mathematics/vector.h>
#include <mathematics/rect.h>

namespace neko
{

    class Circle
    {
	public:
        Vec2f center;
        float radius;
		
        Circle() : center(Vec2f(0,0)), radius(0.0f)
        {
	        
        }
		
        ~Circle() = default;
		
        explicit Circle(Vec2f Center, float Radius) : center(Center), radius(Radius)
        {

        }

        bool Intersects(Circle circle) const
        {
            float distanceVectorX = center.x - circle.center.x;
            float distanceVectorY = center.y - circle.center.y;
            float radiusSum = radius + circle.radius;
        	
            //(x1 - x2)^2 + (y1 - y2)^2 <= (Rad1 + Rad2)^2
            return distanceVectorX * distanceVectorX +
				   distanceVectorY * distanceVectorY <= radiusSum * radiusSum;
        				 
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
    class Plan
    {
    public:
        Vec3f pos;
        Vec3f normal;
		
        Plan() : pos(0), normal(0)
        {
	        
        }
        explicit Plan(Vec3f Pos, Vec3f Normal) : pos(Pos), normal(Normal)
        {

        }
    };
    class Sphere
    {
	public:
        Vec3f center;
        float radius;
        Sphere() : center(Vec3f(0,0,0)), radius(0.0f)
        {
	        
        }
		
        explicit Sphere(Vec3f Center, float Radius) : center(Center), radius(Radius)
        {

        }

        bool Intersects(Sphere sphere) const
        {
            const Vec3f distance = center - sphere.center;

            const float radiusSum = sphere.radius + radius;

            return distance.Magnitude() <= radiusSum;
        }

        bool IsPlanSphereIntersects(Plan plan) const
        {
            const float normMagnitude = plan.normal.Magnitude();
            const float dot = Vec3f::Dot(center - plan.pos, plan.normal);

            const float p = dot / normMagnitude;

            return p < radius && p > -radius;
        }
    };

	class FourCircle
    {
	public:
        std::array<float, 4> centerXs;
        std::array<float, 4> centerYs;
        std::array<float, 4> radius;

		
        FourCircle() noexcept : centerXs{}, centerYs{}, radius{}
        {
	        
        }
		
        ~FourCircle() = default;
		
        explicit FourCircle(std::array<Circle, 4> circles) : centerXs(),centerYs(), radius()
        {
            for (size_t i = 0;  i < 4; i++)
            {
                centerXs[i] = circles[i].center.x;
                centerYs[i] = circles[i].center.y;
                radius[i] = circles[i].radius;
            }
        }

        /*bool RectCircleIntersects(Rect2f rect) const
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
        }*/
        std::array<bool, 4> IntersectsIntrinsics(FourCircle circle);
        std::array<bool, 4> IntersectsCircleIntrinsics(Circle circle);
        std::array<bool, 4> IntersectRectIntrinsics(std::array<Rect2f, 4> rects);

    };
    /*class FourSphere
    {
	public:
        std::array<float, 4> centerXs;
        std::array<float, 4> centerYs;
        std::array<float, 4> centerZs;
        std::array<float, 4> radius;
        
        Sphere() : centerXs{}, centerYs{},centerZs, radius{}
        {
	        
        }
		
        explicit FourSphere(std::array<Sphere, 4> spheres) : centerXs(),centerYs(), centerZs(), radius()
        {
        for (size_t i = 0;  i < 4; i++)
            {
                centerXs[i] = spheres[i].center.x;
                centerYs[i] = spheres[i].center.y;
                centerZs[i] = spheres[i].center.z;
                radius[i] = spheres[i].radius;
            }
        }
    };*/
#ifdef __SSE__

    inline std::array<bool, 4> FourCircle::IntersectsIntrinsics(FourCircle circle)
    {

        /*
	    Vec2f distanceVector = center - circle.center;
	    const float radiusSum = circle.radius + radius;
	    cosnt float radiusSub = circle.radius - radius;
	    
	    (R0 - R1)^2 <= (x1 - x2)^2 + (y1 - y2)^2 <= (Rad1 + Rad2)^2
	    return radiusSub * radiusSub <= distanceVector.SquareMagnitude() <= radiusSum * radiusSum;
        */

        alignas(4 * sizeof(bool))
        std::array<bool, 4> results;
        std::array<float, 4> centers;
        std::array<float, 4> radSum;
        std::array<float, 4> radSub;

        auto x1 = _mm_load_ps(centerXs.data());
        auto y1 = _mm_load_ps(centerYs.data());
        auto rad1 = _mm_load_ps(radius.data());

        auto x2 = _mm_load_ps(circle.centerXs.data());
        auto y2 = _mm_load_ps(circle.centerYs.data());
        auto rad2 = _mm_load_ps(circle.radius.data());

        x1 = _mm_sub_ps(x1, x2);
        y1 = _mm_sub_ps(y1, y2);
        rad1 = _mm_add_ps(rad1, rad2);
        rad1 = _mm_mul_ps(rad1, rad1);

        rad2 = _mm_sub_ps(rad2, rad1);
        rad2 = _mm_mul_ps(rad2, rad2);

        x1 = _mm_mul_ps(x1, x1);
        y1 = _mm_mul_ps(y1, y1);

        x1 = _mm_add_ps(x1, y1);
		
        _mm_store_ps(radSum.data(), rad1);
        _mm_store_ps(radSub.data(), rad2);
        _mm_store_ps(centers.data(), x1);
        
        for (size_t i = 0; i < 4; i++) {
            results[i] = radSub[i] <= centers[i] <= radSum[i];
        }
        // One or more of the distances were less-than-or-equal-to the maximum,
        // so we have something to draw.
        /*
        results[0] = (condition & 0x000F) != 0;
        results[1] = (condition & 0x00F0) != 0;
        results[2] = (condition & 0x0F00) != 0;
        results[3] = (condition & 0xF000) != 0;
        */
    	
        return results;
    }
    inline std::array<bool, 4> FourCircle::IntersectsCircleIntrinsics(Circle circle)
    {
        std::array<Circle, 4> circles;
        circles.fill(circle);
        FourCircle n_circle(circles);

        alignas(4 * sizeof(bool))
        std::array<bool, 4> results;
        std::array<float, 4> centers;
        std::array<float, 4> radSum;
        std::array<float, 4> radSub;
    	
		auto x1 = _mm_load_ps(centerXs.data());
        auto y1 = _mm_load_ps(centerYs.data());
        auto rad1 = _mm_load_ps(radius.data());

        auto x2 = _mm_load_ps(n_circle.centerXs.data());
        auto y2 = _mm_load_ps(n_circle.centerYs.data());
        auto rad2 = _mm_load_ps(n_circle.radius.data());
/*

        x1 = _mm_sub_ps(x1, x2);
        y1 = _mm_sub_ps(y1, y2);
        rad1 = _mm_add_ps(rad1, rad2);

        x1 = _mm_mul_ps(x1, x1);
        y1 = _mm_mul_ps(y1, y1);

        x1 = _mm_add_ps(x1, y1);
    	
        x1 =_mm_sqrt_ps(x1);

        __m128 result = _mm_cmple_ps(x1, rad1);

    	unsigned condition = _mm_movemask_ps(result);

        for (size_t i = 0; i < 4; i++) {
            if (result.m128_f32[i] == 0) {
                results[i] = false;
            }
            else
            {
                results[i] = true;
            }
        }*/
        x1 = _mm_sub_ps(x1, x2);
        y1 = _mm_sub_ps(y1, y2);
        rad1 = _mm_add_ps(rad1, rad2);
        rad1 = _mm_mul_ps(rad1, rad1);

        rad2 = _mm_sub_ps(rad2, rad1);
        rad2 = _mm_mul_ps(rad2, rad2);

        x1 = _mm_mul_ps(x1, x1);
        y1 = _mm_mul_ps(y1, y1);

        x1 = _mm_add_ps(x1, y1);

        _mm_move_ss(x1, y1);
        _mm_move_ss(rad1, rad2);
    	
        __m128 result = _mm_cmple_ps(x1, rad1);

        for (size_t i = 0; i < 4; i++) {
            if (result.m128_f32[i] == 0) {
                results[i] = false;
            }
            else
            {
                results[i] = true;
            }
        // One or more of the distances were less-than-or-equal-to the maximum,
        // so we have something to draw.
        /*
        results[0] = (condition & 0x000F) != 0;
        results[1] = (condition & 0x00F0) != 0;
        results[2] = (condition & 0x0F00) != 0;
        results[3] = (condition & 0xF000) != 0;
        */

        return results;
        //profiler::dumpBlocksToFile("circle_intrasect_profile.prof");
    }

    inline std::array<bool, 4> IntersectRectIntrinsics(std::array<Rect2f, 4> rects)
    {
    	/*
    	 * bool RectCircleIntersects(Rect2f rect) const
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
    	 */
        alignas(4 * sizeof(bool))
        std::array<bool, 4> results;
        std::array<float, 4> centers;
        std::array<float, 4> radSub;

    	
    }

#endif
}