#include <benchmark/benchmark.h>
#include <mathematics/circle.h>
#include <random>
#include <iostream>
#include <mathematics/vector.h>

#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

const long fromRange = 8;
const long toRange = 1 << 18;
const float maxNmb = 50.0f;

float RandomFloat()
{
    static std::random_device rd;
    static std::mt19937 g(rd());
    static std::uniform_real_distribution<float> dist{ -100.0f, 100.0f };

    return dist(g);
}

static void BM_CircleIntersects(benchmark::State& state)
{
	for (auto _ : state)
    {
		const size_t n = state.range(0);
    
        neko::Circle circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat());
        neko::Circle circle2(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat());
    
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(circle.Intersects(circle2));
        }
    }
}
BENCHMARK(BM_CircleIntersects)->Range(fromRange, toRange);

static void BM_CircleIntersectsRSqrt(benchmark::State& state)
{
	for (auto _ : state)
    {
    const size_t n = state.range(0);
    
        neko::Circle circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat());
        neko::Circle circle2(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat());
    
		
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(circle.IntersectsOther(circle2));
        }
    }
}
BENCHMARK(BM_CircleIntersectsRSqrt)->Range(fromRange, toRange);

/*
static void BM_CircleIntersectsIntrinsics(benchmark::State& state)
{

    for (auto _ : state)
    {
        const size_t n = state.range(0) / 4;

        std::array<neko::Circle, 4> array;
        std::array<neko::Circle, 4> array2;

        for (size_t i = 0; i < 4; i++)
        {
            array[i] = neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat());
            array2[i] = neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat());
        }

        neko::FourCircle n_circle(array);
        neko::FourCircle n_circle2(array2);
    	
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(neko::FourCircle::IntersectsIntrinsicsCircle(n_circle, n_circle2));
        }


    }
}
BENCHMARK(BM_CircleIntersectsIntrinsics)->Range(fromRange, toRange);
*/

static void BM_SphereIntersects(benchmark::State& state)
{
	for (auto _ : state)
    {
		const size_t n = state.range(0);
    
        neko::Sphere sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat());
        neko::Sphere sphere2(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat());
    
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(sphere.Intersects(sphere2));
        }
    }
}
BENCHMARK(BM_SphereIntersects)->Range(fromRange, toRange);

static void BM_SphereIntersectsRSqrt(benchmark::State& state)
{
	for (auto _ : state)
    {
		const size_t n = state.range(0);
    
        neko::Sphere sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat());
        neko::Sphere sphere2(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat());
    
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(sphere.IntersectsRSqrt(sphere2));
        }
    }
}
BENCHMARK(BM_SphereIntersectsRSqrt)->Range(fromRange, toRange);

/*
static void BM_SphereIntersectsIntrinsics(benchmark::State& state)
{
    for (auto _ : state)
    {
        const size_t n = state.range(0) / 4;
        
        std::array<neko::Sphere, 4> array;
        std::array<neko::Sphere, 4> array2;

        for (size_t i = 0; i < 4; i++)
        {
            array[i] = neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat());
            array2[i] = neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat());
        }
    	
        neko::FourSphere n_sphere(array);
        neko::FourSphere n_sphere2(array2);

        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(neko::FourSphere::IntersectIntrinsicsSphere(n_sphere, n_sphere2));
        }
    }
}
BENCHMARK(BM_SphereIntersectsIntrinsics)->Range(fromRange, toRange);
*/

