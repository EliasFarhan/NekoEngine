//
// Created by efarhan on 02.12.19.
//
#include <benchmark/benchmark.h>
#include <mathematics/vector.h>
#include <vector>
#include <random>

const long fromRange = 8;
const long toRange = 1<<15;
const float maxNmb = 100.0f;

void RandomFill(std::vector<neko::Vec3f>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(numbers.begin(), numbers.end(), [&g, &dist](){return neko::Vec3f(dist(g), dist(g), dist(g));});
}
void RandomFill(std::vector<neko::Vec4f>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(numbers.begin(), numbers.end(), [&g, &dist](){return neko::Vec4f(dist(g),dist(g),dist(g),0.0f);});
}

static void BM_Vec3Magnitude(benchmark::State& state)
{

    std::vector<neko::Vec3f> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);

    for (auto _ : state)
    {
        for(auto v : local_numbers)
        {
            float f = v.GetMagnitude();
            benchmark::DoNotOptimize(f);
        }
    }
}
BENCHMARK(BM_Vec3Magnitude)->Range(fromRange, toRange);

static void BM_Vec4Magnitude(benchmark::State& state)
{
    std::vector<neko::Vec4f> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);
    for (auto _ : state)
    {
        for(auto v : local_numbers)
        {
            float f = std::sqrt(neko::Vec4f::Dot3(v,v));
            benchmark::DoNotOptimize(f);
        }
    }

}
BENCHMARK(BM_Vec4Magnitude)->Range(fromRange, toRange);

static void BM_Vec3MagnitudeInArray(benchmark::State& state)
{

    std::vector<neko::Vec3f> local_numbers(state.range(0));
    std::vector<float> result(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);

    for (auto _ : state)
    {
        for(size_t i = 0; i < state.range(0);i++)
        {
            result[i] = local_numbers[i].GetMagnitude();
        }
    }
}
BENCHMARK(BM_Vec3MagnitudeInArray)->Range(fromRange, toRange);

static void BM_Vec4MagnitudeInArray(benchmark::State& state)
{
    std::vector<neko::Vec4f> local_numbers(state.range(0));
    std::vector<float> result(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);
    for (auto _ : state)
    {
        for(size_t i = 0; i < state.range(0);i++)
        {
            const neko::Vec4f& v = local_numbers[i];
            result[i] = std::sqrt(neko::Vec4f::Dot3(v,v));
        }
    }
}
BENCHMARK(BM_Vec4MagnitudeInArray)->Range(fromRange, toRange);

BENCHMARK_MAIN();