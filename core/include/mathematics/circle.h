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
	union 
	{
		float radius;
        Vec2f center;

	};
	
    Circle() : center(Vec2f(0, 0)), radius(0.0f)
    {

    }

    ~Circle() = default;

    explicit Circle(Vec2f Center, float Radius) : center(Center), radius(Radius)
    {

    }

    bool Intersects(Circle circle) const
    {
        Vec2f distanceVector = center - circle.center;
        float radiusSum = radius + circle.radius;

        return distanceVector.Magnitude() <= radiusSum;

    }bool IntersectsOther(Circle circle) const
    {
        Vec2f distanceVector = center - circle.center;
        float radiusSum = radius + circle.radius;


        float distance = Sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
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

    static float Sqrt(float x)
    {
        auto i = *reinterpret_cast<int*>(&x);
        i = 0x5f3759df - (i >> 1);
        auto r = *reinterpret_cast<float*>(&i);
        r = r * (1.5f - 0.5f * x * r * r);
        return r * x;
    }

};
struct alignas(8 * sizeof(float)) Plan
{
	union 
	{
		Vec3f pos;
		Vec3f normal;
	};

    Plan() : pos(Vec3f(0, 0, 0)), normal(Vec3f(0, 0, 0))
    {

    }
    explicit Plan(Vec3f Pos, Vec3f Normal) : pos(Pos), normal(Normal)
    {

    }
};

struct alignas(4 * sizeof(float)) Sphere
{
	union 
	{
		Vec3f center;
		float radius;
	};
    

    Sphere() : center(Vec3f(0, 0, 0)), radius(0.0f)
    {

    }

    explicit Sphere(Vec3f Center, float Radius) : center(Center), radius(Radius)
    {

    }

    bool Intersects(const Sphere & sphere) const
    {
        const Vec3f distanceVec = center - sphere.center;

        const float radiusSum = sphere.radius + radius;

        return distanceVec.Magnitude() <= radiusSum;
    }

    bool IntersectsOther(const Sphere & sphere) const
    {
        const Vec3f distanceVec = center - sphere.center;

        const float radiusSum = sphere.radius + radius;

        float distance = Circle::Sqrt(distanceVec.x * distanceVec.x +
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
};

class FourCircle
{
public:
    alignas(4 * sizeof(float)) std::array<float, 4> centerXs;
    alignas(4 * sizeof(float)) std::array<float, 4> centerYs;
    alignas(4 * sizeof(float)) std::array<float, 4> radius;

    FourCircle() noexcept : centerXs{}, centerYs{}, radius{}
    {

    }

    ~FourCircle() = default;

    explicit FourCircle(std::array<Circle, 4> circles) : centerXs(), centerYs(), radius()
    {
        for (size_t i = 0; i < 4; i++)
        {
            centerXs[i] = circles[i].center.x;
            centerYs[i] = circles[i].center.y;
            radius[i] = circles[i].radius;
        }
    }

    std::array<bool, 4> IntersectsIntrinsics(const FourCircle & circle) const;
    std::array<bool, 4> IntersectsCircleIntrinsics(const Circle & circle, size_t n) const;
};

class FourPlan
{
public:
    std::array<float, 4> posXs;
    std::array<float, 4> posYs;
    std::array<float, 4> posZs;

    std::array<float, 4> normalXs;
    std::array<float, 4> normalYs;
    std::array<float, 4> normalZs;

    FourPlan() = default;

    explicit FourPlan(std::array<Plan, 4> plans) noexcept : posXs{}, posYs{}, posZs{}, normalXs{}, normalYs{}, normalZs{}
    {
        for (size_t i = 0; i < 4; i++)
        {
            posXs[i] = plans[i].pos.x;
            posYs[i] = plans[i].pos.y;
            posZs[i] = plans[i].pos.z;

            normalXs[i] = plans[i].normal.x;
            normalYs[i] = plans[i].normal.y;
            normalZs[i] = plans[i].normal.z;
        }
    }

};

class alignas(4 * sizeof(float)) FourSphere
{
public:
    std::array<float, 4> centerXs;
    std::array<float, 4> centerYs;
    std::array<float, 4> centerZs;
    std::array<float, 4> radius;

    FourSphere() = default;

    explicit FourSphere(std::array<Sphere, 4> & spheres) : centerXs(), centerYs(), centerZs(), radius()
    {
        for (size_t i = 0; i < 4; i++)
        {
            centerXs[i] = spheres[i].center.x;
            centerYs[i] = spheres[i].center.y;
            centerZs[i] = spheres[i].center.z;
            radius[i] = spheres[i].radius;
        }
    }

    std::array<bool, 4> Intersects(const FourSphere & sphere)
    {
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
            mag[i] = Circle::Sqrt(distanceX[i] * distanceX[i] + distanceY[i] * distanceY[i] + distanceZ[i] * distanceZ[i]);

            results[i] = radSub[i] <= mag[i] <= radSum[i];
        }

        return results;
    }

    std::array<bool, 4> IntersectIntrinsics(const FourSphere & spheres) const;
    std::array<bool, 4> IntersectSphereIntrinsics(Sphere & sphere);
    std::array<bool, 4> IntersectSpherePlanIntrinsics(Plan & plan);
};

#ifdef __SSE__

inline std::array<bool, 4> FourCircle::IntersectsIntrinsics(const FourCircle& circle) const
{
    alignas(4 * sizeof(bool)) std::array<bool, 4> results = {};
    alignas(4 * sizeof(float)) std::array<float, 4> magnitude = {};
    alignas(4 * sizeof(float)) std::array<float, 4> radii = {};

    __m128 x1;
    __m128 y1;
    __m128 distx;
    __m128 rad1;

    __m128 x2;
    __m128 y2;
    __m128 disty;
    __m128 rad2;

    for (size_t i = 0; i < 4; i++)
    {
        x1 = _mm_loadu_ps(centerXs.data() + i);
        y1 = _mm_loadu_ps(centerYs.data() + i);
        rad1 = _mm_loadu_ps(radius.data() + i);

        x2 = _mm_loadu_ps(circle.centerXs.data() + i);
        y2 = _mm_loadu_ps(circle.centerYs.data() + i);
        rad2 = _mm_loadu_ps(circle.radius.data() + i);
        x1 = _mm_sub_ps(x1, x2);
        y1 = _mm_sub_ps(y1, y2);

        rad1 = _mm_add_ps(rad1, rad2);

        distx = _mm_mul_ps(x1, x1);
        disty = _mm_mul_ps(y1, y1);

        distx = _mm_add_ps(distx, disty);

        auto mag = _mm_rsqrt_ps(distx);
    	
        magnitude[i] = _mm_movemask_ps(mag);
        radii[i] = _mm_movemask_ps(distx);

        results[i] = magnitude[i] <= radii[i];
    }
    return results;
}

inline std::array<bool, 4> FourCircle::IntersectsCircleIntrinsics(const Circle& circle, size_t n) const
{

    std::array<Circle, 4> circles;
    circles.fill(circle);
    FourCircle n_circle(circles);

    alignas(4 * sizeof(bool)) std::array<bool, 4> results = {};
    alignas(4 * sizeof(float)) std::array<float, 4> magnitude = {};
    alignas(4 * sizeof(float)) std::array<float, 4> radii = {};

    __m128 x1;
    __m128 y1;
    __m128 distx;
    __m128 rad1;

    __m128 x2;
    __m128 y2;
    __m128 disty;
    __m128 rad2;

    for (size_t i = 0; i < 4; i++)
    {
        x1 = _mm_loadu_ps(centerXs.data() + i);
        y1 = _mm_loadu_ps(centerYs.data() + i);
        rad1 = _mm_loadu_ps(radius.data() + i);

        x2 = _mm_loadu_ps(n_circle.centerXs.data() + i);
        y2 = _mm_loadu_ps(n_circle.centerYs.data() + i);
        rad2 = _mm_loadu_ps(n_circle.radius.data() + i);
        x1 = _mm_sub_ps(x1, x2);
        y1 = _mm_sub_ps(y1, y2);

        rad1 = _mm_add_ps(rad1, rad2);

        distx = _mm_mul_ps(x1, x1);
        disty = _mm_mul_ps(y1, y1);

        distx = _mm_add_ps(distx, disty);

        auto mag = _mm_rsqrt_ps(distx);

        magnitude[i] = _mm_movemask_ps(mag);
        radii[i] = _mm_movemask_ps(distx);

        results[i] = magnitude[i] <= radii[i];
    }
    return results;
}

inline std::array<bool, 4> FourSphere::IntersectIntrinsics(const FourSphere& spheres) const
{
    __m128 x1, x2, y1, y2, z1, z2, rad1, rad2;
    int result = 0;
    alignas(4 * sizeof(bool))	std::array<bool, 4> results = {};
    alignas(4 * sizeof(float))	std::array<float, 4> magnitude = {};
    alignas(4 * sizeof(float))	std::array<float, 4> radii = {};

        x1 = _mm_load_ps(centerXs.data());
        x2 = _mm_load_ps(spheres.centerXs.data());
        auto x3 = _mm_sub_ps(x1, x2); // center.x - sphere.center.x
        x3 = _mm_sub_ps(x3, x3);

        y1 = _mm_load_ps(centerYs.data());
        y2 = _mm_load_ps(spheres.centerYs.data());
        auto y3 = _mm_sub_ps(y1, y2); //center.y - sphere.center.y
        y3 = _mm_sub_ps(y3, y3);

        z1 = _mm_load_ps(centerZs.data());
        z2 = _mm_load_ps(spheres.centerZs.data());
        auto z3 = _mm_sub_ps(z1, z2); //center.z - sphere.center.z
        z3 = _mm_sub_ps(z3, z3);

        rad1 = _mm_load_ps(radius.data());
        rad2 = _mm_load_ps(spheres.radius.data());
        auto rad3 = _mm_add_ps(rad1, rad2); // radius + sphere.radius
    	
        auto dist1 = _mm_add_ps(y3, z3);
        auto dist2 = _mm_add_ps(x3, dist1);
        dist2 = _mm_mul_ps(dist2, dist2);

        auto mag = _mm_rsqrt_ps(dist2);
        auto mask = _mm_cmple_ps(rad3, mag);
        result = _mm_movemask_ps(mask);
    	
    if (result & 1) { results[0] = true; }
    if (result & 2) { results[1] = true; }
    if (result & 4) { results[2] = true; }
    if (result & 8) { results[3] = true; }
    
	
    return results;
}

inline std::array<bool, 4> FourSphere::IntersectSphereIntrinsics(Sphere& sphere)
{
    std::array<Sphere, 4> spheres;
    spheres.fill(sphere);
    FourSphere n_sphere(spheres);
    __m128 x1, x2, y1, y2, z1, z2, rad1, rad2;
    int result = 0;
    alignas(4 * sizeof(bool))	std::array<bool, 4> results = {};
    alignas(4 * sizeof(float))	std::array<float, 4> magnitude = {};
    alignas(4 * sizeof(float))	std::array<float, 4> radii = {};

        x1 = _mm_load_ps(centerXs.data());
        x2 = _mm_load_ps(n_sphere.centerXs.data());
        auto x3 = _mm_sub_ps(x1, x2); // center.x - sphere.center.x
        x3 = _mm_sub_ps(x3, x3);

        y1 = _mm_load_ps(centerYs.data());
        y2 = _mm_load_ps(n_sphere.centerYs.data());
        auto y3 = _mm_sub_ps(y1, y2); //center.y - sphere.center.y
        y3 = _mm_sub_ps(y3, y3);

        z1 = _mm_load_ps(centerZs.data());
        z2 = _mm_load_ps(n_sphere.centerZs.data());
        auto z3 = _mm_sub_ps(z1, z2); //center.z - sphere.center.z
        z3 = _mm_sub_ps(z3, z3);

        rad1 = _mm_load_ps(radius.data());
        rad2 = _mm_load_ps(n_sphere.radius.data());
        auto rad3 = _mm_add_ps(rad1, rad2); // radius + sphere.radius

        auto dist1 = _mm_add_ps(y3, z3);
        auto dist2 = _mm_add_ps(x3, dist1);
        dist2 = _mm_mul_ps(dist2, dist2);

        auto mag = _mm_rsqrt_ps(dist2);
        auto mask = _mm_cmple_ps(rad3, mag);
        result = _mm_movemask_ps(mask);

    
    // One or more of the distances were less-than-or-equal-to the maximum,
    // so we have something to draw.
    if (result & 1) { results[0] = true; }
    if (result & 2) { results[1] = true; }
    if (result & 4) { results[2] = true; }
    if (result & 8) { results[3] = true; }
	
    return results;
}


inline std::array<bool, 4> FourSphere::IntersectSpherePlanIntrinsics(Plan& plan)
{
    /*
        const float normMagnitude = plan.normal.Magnitude();
        const float dot = Vec3f::Dot(center - plan.pos, plan.normal);

        const float p = dot / normMagnitude;

        return p < radius && p > -radius;
    */
    std::array<Plan, 4> array;
    array.fill(plan);
    FourPlan n_plan(array);

    alignas(4 * sizeof(bool)) std::array<bool, 4> results = {};

    auto normX = _mm_load_ps(n_plan.normalXs.data());
    auto normY = _mm_load_ps(n_plan.normalYs.data());
    auto normZ = _mm_load_ps(n_plan.normalZs.data());

    auto posX = _mm_load_ps(n_plan.posXs.data());
    auto posY = _mm_load_ps(n_plan.posYs.data());
    auto posZ = _mm_load_ps(n_plan.posZs.data());

    auto centerX = _mm_load_ps(centerXs.data());
    auto centerY = _mm_load_ps(centerYs.data());
    auto centerZ = _mm_load_ps(centerZs.data());

    auto rad = _mm_load_ps(radius.data());

    posX = _mm_sub_ps(posX, centerX); //center - plan.pos = pos
    posY = _mm_sub_ps(posY, centerY);
    posZ = _mm_sub_ps(posZ, centerZ);

    normX = _mm_mul_ps(normX, posX); //dot pos, plan.norm = norm
    normY = _mm_mul_ps(normY, posY);
    normZ = _mm_mul_ps(normZ, posZ);

    normY = _mm_sub_ps(normY, normZ);
    normX = _mm_sub_ps(normX, normY);//norm - norm - norm

    posX = _mm_div_ps(posX, normX);

    __m128 mask = _mm_cmple_ps(posX, rad);

    int result = _mm_movemask_ps(mask);

    if (result & 1) { results[0] = true; }
    if (result & 2) { results[1] = true; }
    if (result & 4) { results[2] = true; }
    if (result & 8) { results[3] = true; }

    return results;
}
#endif

}
