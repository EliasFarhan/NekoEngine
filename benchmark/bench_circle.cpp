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
	float p = RandomFloat();
	std::vector<neko::Circle<neko::Vec2f>> v1(n, neko::Circle(neko::Vec2f(p, p), p));
	std::vector<neko::Circle<neko::Vec2f>> v2(n, neko::Circle(neko::Vec2f(p, p), p));
	
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
    float p = RandomFloat();
    std::vector<neko::Circle<neko::Vec3f>> v1(n, neko::Circle(neko::Vec3f(p, p,p), p));
    std::vector<neko::Circle<neko::Vec3f>> v2(n, neko::Circle(neko::Vec3f(p, p,p), p));

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
    float p = RandomFloat();
    std::vector<neko::Circle<neko::Vec2f>> v1(n, neko::Circle(neko::Vec2f(p, p), p));
    std::vector<neko::Rect<neko::Vec2f>> v2(n, neko::Rect(neko::Vec2f(p, p), neko::Vec2f(p,p)));

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
    float p = RandomFloat();
    std::vector<neko::Circle<neko::Vec3f>> v1(n, neko::Circle(neko::Vec3f(p, p,p), p));

    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(neko::Sphere3D::IsPlanCircleIntersects(v1[i], neko::Vec3f(0,1,0), neko::Vec3f(p,p,p)));
        }
    }
}
BENCHMARK(BM_PlanCircleIntersects)->Range(fromRange, toRange);