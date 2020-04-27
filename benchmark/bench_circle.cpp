#include <benchmark/benchmark.h>
#include <mathematics/circle.h>
#include <random>
#include <iostream>
#include <mathematics/vector.h>

#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

const long fromRange = 8;
const long toRange = 1 << 16;
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
    const size_t n = state.range(0);
    std::vector<neko::Circle> v1;
	std::vector<neko::Circle> v2;
    v1.reserve(n);
	v2.reserve(n);

    for (size_t i = 0; i < n; i++)
    {
        v1.push_back( neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat()));
        v2.push_back( neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat()));
    }
	
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].Intersects(v2[i]));
        }
    }
}
BENCHMARK(BM_CircleIntersects)->Range(fromRange, toRange);

static void BM_CircleIntersectsOther(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Circle> v1;
	std::vector<neko::Circle> v2;
    v1.reserve(n);
	v2.reserve(n);

    for (size_t i = 0; i < n; i++)
    {
        v1.push_back( neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat()));
        v2.push_back( neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat()));
    }
	
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].IntersectsOther(v2[i]));
        }
    }
}
BENCHMARK(BM_CircleIntersectsOther)->Range(fromRange, toRange);

static void BM_CircleIntersectsArray(benchmark::State& state)
{
    const size_t n = state.range(0) / 4;
    std::vector<neko::FourCircle> vec;
    std::vector<neko::FourCircle> vec2;

    std::array<neko::Circle, 4> array;
    std::array<neko::Circle, 4> array2;

    vec.reserve(n);
    vec2.reserve(n);

    for (size_t i = 0; i < n; i++)
    {
        for (size_t i = 0; i < 4; i++)
        {
            array[i] = neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat());
        
            array2[i] = neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat());
        }


        neko::FourCircle n_circle(array);
        neko::FourCircle n_circle2(array2);

        vec.push_back(n_circle);
        vec2.push_back(n_circle2);
    }

    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(vec[i].Intersects(vec2[i]));
        }

    }
}
BENCHMARK(BM_CircleIntersectsArray)->Range(fromRange, toRange);

static void BM_CircleIntersectsIntrinsics(benchmark::State& state)
{


    const size_t n = state.range(0) / 4;
    std::vector<neko::FourCircle> vec;
    std::vector<neko::FourCircle> vec2;

    std::array<neko::Circle, 4> array;
    std::array<neko::Circle, 4> array2;

    vec.reserve(n);
    vec2.reserve(n);

    for (size_t i = 0; i < n; i++)
    {
        for (size_t i = 0; i < 4; i++)
        {
            array[i] = neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat());
        
            array2[i] = neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat());
        }


        neko::FourCircle n_circle(array);
        neko::FourCircle n_circle2(array2);

        vec.push_back(n_circle);
        vec2.push_back(n_circle2);
    }
	
    for (auto _ : state)
    {
        for (size_t i = 0; i < n ; i++)
        {
            benchmark::DoNotOptimize(neko::FourCircle::IntersectsIntrinsicsCircle(vec[i],vec2[i]));
        }


    }
}
BENCHMARK(BM_CircleIntersectsIntrinsics)->Range(fromRange, toRange);



static void BM_SphereIntersects(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Sphere> v1;
    std::vector<neko::Sphere> v2;
    v1.reserve(n);
    v2.reserve(n);

    for (size_t i = 0; i < n; i++)
    {
        v1.push_back(neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat()));
        v2.push_back(neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat()));
    }
	
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].Intersects(v2[i]));
        }
    }
}
BENCHMARK(BM_SphereIntersects)->Range(fromRange, toRange);

static void BM_SphereIntersectsOther(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Sphere> v1;
    std::vector<neko::Sphere> v2;
    v1.reserve(n);
    v2.reserve(n);

    for (size_t i = 0; i < n; i++)
    {
        v1.push_back(neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat()));
        v2.push_back(neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat()));
    }
	
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].IntersectsOther(v2[i]));
        }
    }
}
BENCHMARK(BM_SphereIntersectsOther)->Range(fromRange, toRange);

static void BM_SphereIntersectsIntrinsics(benchmark::State& state)
{
    const size_t n = state.range(0) / 4;
    std::vector<neko::FourSphere> vec;
    std::vector<neko::FourSphere> vec2;

    std::array<neko::Sphere, 4> array;
    std::array<neko::Sphere, 4> array2;

    vec.reserve(n);
    vec2.reserve(n);

    for (size_t i = 0; i < n; i++)
    {
        for (size_t i = 0; i < 4; i++)
        {
            array[i] = neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat());
        }
        for (size_t i = 0; i < 4; i++)
        {
            array2[i] = neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat());
        }


        neko::FourSphere n_sphere(array);
        neko::FourSphere n_sphere2(array2);

        vec.push_back(n_sphere);
        vec2.push_back(n_sphere2);
    }
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(neko::FourSphere::IntersectIntrinsicsSphere(vec[i], vec2[i]));
        }
    }
}
BENCHMARK(BM_SphereIntersectsIntrinsics)->Range(fromRange, toRange);

static void BM_SphereIntersectsArray(benchmark::State& state)
{
    const size_t n = state.range(0) / 4;
    std::vector<neko::FourSphere> vec;
    std::vector<neko::FourSphere> vec2;

    std::array<neko::Sphere, 4> array;
    std::array<neko::Sphere, 4> array2;

    vec.reserve(n);
    vec2.reserve(n);

    for (size_t i = 0; i < n; i++)
    {
        for (size_t i = 0; i < 4; i++)
        {
            array[i] = neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat());
        }
        for (size_t i = 0; i < 4; i++)
        {
            array2[i] = neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat());
        }


        neko::FourSphere n_sphere(array);
        neko::FourSphere n_sphere2(array2);

        vec.push_back(n_sphere);
        vec2.push_back(n_sphere2);
    }
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(vec[i].Intersects(vec2[i]));
        }
    }
}
BENCHMARK(BM_SphereIntersectsArray)->Range(fromRange, toRange);
