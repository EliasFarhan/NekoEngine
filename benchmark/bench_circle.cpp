#include <benchmark/benchmark.h>
#include <mathematics/circle.h>
#include <random>
#include <iostream>

const long fromRange = 8;
const long toRange = 1 << 17;
const float maxNmb = 100.0f;

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

static void BM_RectCircleIntersects(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Circle> v1;
    std::vector<neko::Rect2f> v2;
    v1.reserve(n);
    v2.reserve(n);
	
    for (size_t i = 0; i < n; i++)
    {
        v1.push_back(neko::Circle(neko::Vec2f(RandomFloat(), RandomFloat()), RandomFloat()));
        v2.push_back(neko::Rect2f(neko::Vec2f(RandomFloat(), RandomFloat()), neko::Vec2f(RandomFloat(), RandomFloat())));
    }
	
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].RectCircleIntersects(v2[i]));
        }
    }
}
BENCHMARK(BM_RectCircleIntersects)->Range(fromRange, toRange);

static void BM_PlanCircleIntersects(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Sphere> v1;
    std::vector<neko::Plan> v2;
    v1.reserve(n);
    v2.reserve(n);

    for (size_t i = 0; i < n; i++)
    {
        v1.push_back(neko::Sphere(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), RandomFloat()));
        v2.push_back(neko::Plan(neko::Vec3f(RandomFloat(), RandomFloat(), RandomFloat()), neko::Vec3f(RandomFloat(),RandomFloat(),RandomFloat())));
    }
	
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].IsPlanCircleIntersects(v2[i]));
        }
    }
}
BENCHMARK(BM_PlanCircleIntersects)->Range(fromRange, toRange);