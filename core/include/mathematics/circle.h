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
#include <mathematics/intrinsincs.h>

namespace neko
{

struct Circle
{
    float radius;
    Vec2f center;

Circle() : radius(0.0f), center(Vec2f(0, 0))
{

}

~Circle() = default;

explicit Circle(Vec2f Center, float Radius) : radius(Radius), center(Center)
{

}

bool Intersects(Circle circle) const
{
    const Vec2f distanceVector = center - circle.center;
    const float radiusSum = radius + circle.radius;

    return distanceVector.Magnitude() <= radiusSum;
}
bool IntersectsOther(Circle circle) const
{
    const Vec2f distanceVector = center - circle.center;
    const float radiusSum = radius + circle.radius;

    const float distance = RSqrt(distanceVector.x * distanceVector.x + 
								   distanceVector.y * distanceVector.y);
    return distance <= radiusSum;

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

/*
 *TODO : Circle ray intersection
bool IntersectsRay(Vec2f pos,float direction) const
{
	const auto a = Vec2f::Dot(pos, pos);
    const auto b = Vec2f::Dot(center,pos);
    const auto c = Vec2f::Dot(center, center) - radius * radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {  }
    else
    {
        discriminant = sqrt(discriminant);

        float t1 = (-b - discriminant) / (2 * a);
        float t2 = (-b + discriminant) / (2 * a);

        if (t1 >= 0 && t1 <= 1)
        {
            return true;
        }
        if (t2 >= 0 && t2 <= 1)
        {
            return true;
        }
        return false;
    }
}
*/	
};
	
struct Plan
{
	Vec3f pos;
	Vec3f normal;
	
    Plan() : pos(Vec3f(0, 0, 0)), normal(Vec3f(0, 0, 0))
    {

    }
    explicit Plan(Vec3f Pos, Vec3f Normal) : pos(Pos), normal(Normal)
    {

    }
};

struct Sphere
{
    Vec3f center;
    float radius;

    Sphere() : center(Vec3f(0, 0, 0)), radius(0.0f)
    {

    }

    explicit Sphere(Vec3f Center, float Radius) : center(Center), radius(Radius)
    {

    }

    bool Intersects(const Sphere sphere) const
    {
        const Vec3f distanceVec = center - sphere.center;

        const float radiusSum = sphere.radius + radius;

        return distanceVec.Magnitude() <= radiusSum;
    }

    bool IntersectsOther(const Sphere sphere) const
    {
        const Vec3f distanceVec = center - sphere.center;

        const float radiusSum = sphere.radius + radius;

        const float distance = RSqrt(distanceVec.x * distanceVec.x +
					                   distanceVec.y * distanceVec.y +
					                   distanceVec.z * distanceVec.z);

        return distance <= radiusSum;
    }

    bool IsPlanSphereIntersects(Plan plan) const
    {
        const float normMagnitude = plan.normal.Magnitude();
        const float dot = Vec3f::Dot(center - plan.pos, plan.normal);

        const float p = dot / normMagnitude;

        return p < radius&& p > -radius;
    }
	
    /*
     *TODO : Sphere ray intersection
    bool IntersectsRay(Vec3f start, Vec3f end) const
    {
	    const Vec3f d = end - start;
	    const Vec3f f = center - start;

	    const float a = Vec3f::Dot(d,d);
	    const float b = 2 * Vec3f::Dot(f,d);
	    const float c = Vec3f::Dot(f,f) - radius * radius;

        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0){}
        else
        {
            discriminant = sqrt(discriminant);
            float t1 = (-b - discriminant) / (2 * a);
            float t2 = (-b + discriminant) / (2 * a);

            if (t1 >= 0 && t1 <= 1)
            {
                return true;
            }
            if (t2 >= 0 && t2 <= 1)
            {
                // ExitWound
                return true;
            }

            return false;
        }
	    return false;
    }
    */
};

class alignas(4 * sizeof(float)) FourCircle
{
public:
    std::array<float, 4> centerXs;
    std::array<float, 4> centerYs;
    std::array<float, 4> radius;

    FourCircle() noexcept : centerXs{}, centerYs{}, radius{}
    {

    }

    ~FourCircle() = default;

    explicit FourCircle(std::array<Circle, 4>& circles) : centerXs(), centerYs(), radius()
    {
        for (size_t i = 0; i < 4; i++)
        {
            centerXs[i] = circles[i].center.x;
            centerYs[i] = circles[i].center.y;
            radius[i] = circles[i].radius;
        }
    }
    std::array<bool, 4> Intersects(const FourCircle& sphere)
    {
        alignas(4 * sizeof(float))
        std::array<float, 4> distanceX = {};
        std::array<float, 4> distanceY = {};
        std::array<float, 4> radSum = {};
        std::array<float, 4> radSub = {};
        std::array<float, 4> mag = {};
        std::array<bool, 4> results = {};

        for (size_t i = 0; i < 4; i++)
        {
            distanceX[i] = centerXs[i] - sphere.centerXs[i];
            distanceY[i] = centerYs[i] - sphere.centerYs[i];

            radSum[i] = radius[i] + sphere.radius[i];
            radSub[i] = radius[i] - sphere.radius[i];

            mag[i] = RSqrt(distanceX[i] * distanceX[i] + distanceY[i] * distanceY[i]);

            results[i] = radSub[i] <= mag[i] <= radSum[i];
        }

        return results;
    }
    static uint8_t IntersectsIntrinsicsCircle(const FourCircle& circlesA, const FourCircle& circlesB);
};

struct alignas(4 * sizeof(float)) FourSphere
{
    std::array<float, 4> centerXs;
    std::array<float, 4> centerYs;
    std::array<float, 4> centerZs;
    std::array<float, 4> radius;

    FourSphere() noexcept : centerXs{}, centerYs{}, centerZs{}, radius{}
    {

    }
	
	~FourSphere() = default;
	
    explicit FourSphere(std::array<Sphere, 4>& spheres) : centerXs(), centerYs(), centerZs(), radius()
    {
        for (size_t i = 0; i < 4; i++)
        {
            centerXs[i] = spheres[i].center.x;
            centerYs[i] = spheres[i].center.y;
            centerZs[i] = spheres[i].center.z;
            radius[i] = spheres[i].radius;
        }
    }

    std::array<bool, 4> Intersects(const FourSphere& sphere)
    {
        alignas(4 * sizeof(float))
        std::array<float, 4> distanceX = {};
        std::array<float, 4> distanceY = {};
        std::array<float, 4> distanceZ = {};
        std::array<float, 4> radSum = {};
        std::array<float, 4> radSub = {};
        std::array<float, 4> mag = {};
        std::array<bool, 4> results = {};

        for (size_t i = 0; i < 4; i++)
        {
            distanceX[i] = centerXs[i] - sphere.centerXs[i];
            distanceY[i] = centerYs[i] - sphere.centerYs[i];
            distanceZ[i] = centerZs[i] - sphere.centerZs[i];

            radSum[i] = radius[i] + sphere.radius[i];
            radSub[i] = radius[i] - sphere.radius[i];

            mag[i] = RSqrt(distanceX[i] * distanceX[i] + distanceY[i] * distanceY[i] + distanceZ[i] * distanceZ[i]);

            results[i] = radSub[i] <= mag[i] <= radSum[i];
        }

        return results;
    }
    static uint8_t IntersectIntrinsicsSphere(const FourSphere & spheresA, const FourSphere& spheresB);
};

#ifdef __SSE__
/**
* \brief Test the intersection between four circle in a list.
*/
inline uint8_t FourCircle::IntersectsIntrinsicsCircle(const FourCircle& circlesA, const FourCircle& circlesB)
{
    __m128 x1 = _mm_load_ps(circlesA.centerXs.data());
    __m128 y1 = _mm_load_ps(circlesA.centerYs.data());
    __m128 rad1 = _mm_load_ps(circlesA.radius.data());

    __m128 x2 = _mm_load_ps(circlesB.centerXs.data());
    __m128 y2 = _mm_load_ps(circlesB.centerYs.data());
    __m128 rad2 = _mm_load_ps(circlesB.radius.data());
    x1 = _mm_sub_ps(x1, x2);
    y1 = _mm_sub_ps(y1, y2);

    __m128 radSum = _mm_add_ps(rad1, rad2);
    __m128 radSub = _mm_sub_ps(rad2, rad1);

    __m128 distx = _mm_mul_ps(x1, x1);
    __m128 disty = _mm_mul_ps(y1, y1);

    distx = _mm_add_ps(distx, disty);

    __m128 mag = _mm_rsqrt_ps(distx);
    __m128 mask = _mm_cmpneq_ps(_mm_cmple_ps(radSum, mag), _mm_cmple_ps(radSub, mag));

    uint8_t results = _mm_movemask_ps(mask);
    
    return results;
}
	
/**
* \brief Test the intersection between four sphere in a list.
*/

inline uint8_t FourSphere::IntersectIntrinsicsSphere(const FourSphere& spheresA, const FourSphere& spheresB)
{
    __m128 x1 = _mm_load_ps(spheresA.centerXs.data());
    __m128 x2 = _mm_load_ps(spheresB.centerXs.data());

    __m128 y1 = _mm_load_ps(spheresA.centerYs.data());
    __m128 y2 = _mm_load_ps(spheresB.centerYs.data());

    __m128 z1 = _mm_load_ps(spheresA.centerZs.data());
    __m128 z2 = _mm_load_ps(spheresB.centerZs.data());

    __m128 rad1 = _mm_load_ps(spheresA.radius.data());
    __m128 rad2 = _mm_load_ps(spheresB.radius.data());

    __m128 radSum = _mm_add_ps(rad1, rad2); // radius + sphere.radius
    __m128 radSub = _mm_sub_ps(rad2, rad1); // radius - sphere.radius
    __m128 x3 = _mm_sub_ps(x1, x2); // center.x - sphere.center.x
    __m128 y3 = _mm_sub_ps(y1, y2); //center.y - sphere.center.y
    __m128 z3 = _mm_sub_ps(z1, z2); //center.z - sphere.center.z

    __m128 distx = _mm_mul_ps(x3, x3);
    __m128 disty = _mm_mul_ps(y3, y3);
    __m128 distz = _mm_mul_ps(z3, z3);

    auto dist = _mm_add_ps(distx, _mm_add_ps(disty, distz));

    auto mag = _mm_rsqrt_ps(dist);
    auto mask = _mm_cmpneq_ps(_mm_cmple_ps(radSum, mag), _mm_cmple_ps(radSub, mag));
    uint8_t results = _mm_movemask_ps(mask);

    return results;
}

#endif
}