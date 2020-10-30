#include <benchmark/benchmark.h>
#include <random>
#include <algorithm>
#include <mathematics/aabb.h>
#include <random_fill.h>

const unsigned long fromRange = 2;
const unsigned long toRange = 1 << 20;

static void BM_Aabb2CheckIntersect(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Aabb2d> v1(n);
    std::vector<neko::Aabb2d> v2(n);
    for (auto v : v1) {
        v.FromCenterExtends(neko::Vec2f(RandomFloat(), RandomFloat()), neko::Vec2f(RandomFloat(), RandomFloat()));
    }
    for (auto v : v2) {
        v.FromCenterExtends(neko::Vec2f(RandomFloat(), RandomFloat()), neko::Vec2f(RandomFloat(), RandomFloat()));
    }
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].IntersectAabb(v2[i]));
        }
    }
}

BENCHMARK(BM_Aabb2CheckIntersect)->Range(fromRange, toRange);

static void BM_Aabb2CheckContains(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Aabb2d> v1(n);
    std::vector<neko::Aabb2d> v2(n);
    for (auto v : v1) {
        v.FromCenterExtends(neko::Vec2f(RandomFloat(), RandomFloat()), neko::Vec2f(RandomFloat(), RandomFloat()));
    }
    for (auto v : v2) {
        v.FromCenterExtends(neko::Vec2f(RandomFloat(), RandomFloat()), neko::Vec2f(RandomFloat(), RandomFloat()));
    }
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].ContainsAabb(v2[i]));
        }
    }
}

BENCHMARK(BM_Aabb2CheckContains)->Range(fromRange, toRange);

BENCHMARK_MAIN();