#include <benchmark/benchmark.h>
#include <random>
#include <algorithm>
#include <numeric>
#include <vector>
#include <mathematics/vec4.h>

const long fromRange = 8;
const long toRange = 1 << 15;
const float start = -100.0f;
const float end = 100.0f;

float accumulate_magnitude_v3(const std::vector<neko::Vec3f>& v)
{
    return std::accumulate(v.cbegin(), v.cend(), 0.0f, [](float total, const neko::Vec3f& b)
    { return total + b.Magnitude(); });
}

float accumulate_magnitude_v4(const std::vector<neko::Vec4f>& v)
{
    return std::accumulate(v.cbegin(), v.cend(), 0.0f, [](float total, const neko::Vec4f& b)
    { return total + b.Magnitude(); });
}

float accumulate_then_magnitude_v3(const std::vector<neko::Vec3f>& v)
{
    const auto total= std::accumulate(v.cbegin(), v.cend(), neko::Vec3f::zero());
    return total.Magnitude();
}

float accumulate_then_magnitude_v4(const std::vector<neko::Vec4f>& v)
{
    const auto total =  std::accumulate(v.cbegin(), v.cend(), neko::Vec4f::zero());
    return total.Magnitude();
}

static void BM_Vec3fAccumulateMagnitude(benchmark::State& state)
{
    std::vector<neko::Vec3f> v(state.range(0));
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(v.begin(), v.end(), [&g, &dist]()
    { return neko::Vec3f(dist(g), dist(g), dist(g)); });
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(accumulate_magnitude_v3(v));

    }
}

BENCHMARK(BM_Vec3fAccumulateMagnitude)->Range(fromRange, toRange);

static void BM_Vec4fAccumulateMagnitude(benchmark::State& state)
{
    std::vector<neko::Vec4f> v(state.range(0));
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(v.begin(), v.end(), [&g, &dist]()
    { return neko::Vec4f(dist(g), dist(g), dist(g), 0.0f); });
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(accumulate_magnitude_v4(v));

    }
}

BENCHMARK(BM_Vec4fAccumulateMagnitude)->Range(fromRange, toRange);

static void BM_Vec3fAccumulateThenMagnitude(benchmark::State& state)
{
    std::vector<neko::Vec3f> v(state.range(0));
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(v.begin(), v.end(), [&g, &dist]()
    { return neko::Vec3f(dist(g), dist(g), dist(g)); });
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(accumulate_then_magnitude_v3(v));

    }
}

BENCHMARK(BM_Vec3fAccumulateThenMagnitude)->Range(fromRange, toRange);

static void BM_Vec4fAccumulateThenMagnitude(benchmark::State& state)
{
    std::vector<neko::Vec4f> v(state.range(0));
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(v.begin(), v.end(), [&g, &dist]()
    { return neko::Vec4f(dist(g), dist(g), dist(g), 0.0f); });
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(accumulate_then_magnitude_v4(v));

    }
}

BENCHMARK(BM_Vec4fAccumulateThenMagnitude)->Range(fromRange, toRange);

BENCHMARK_MAIN();