//
// Created by efarhan on 02.12.19.
//
#include <benchmark/benchmark.h>
#include <mathematics/vector.h>
#include "mathematics/vector_nvec.h"
#include <vector>
#include <random>
#include <iostream>

#include "engine/assert.h"

const long fromRange = 8;
const long toRange = 1 << 15;
const float maxNmb = 100.0f;


void RandomFill(std::vector<neko::Vec3f>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(numbers.begin(), numbers.end(), [&g, &dist]() { return neko::Vec3f(dist(g), dist(g), dist(g)); });
}

void RandomFill(std::vector<neko::Vec4f>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(numbers.begin(), numbers.end(),
                  [&g, &dist]() { return neko::Vec4f(dist(g), dist(g), dist(g), 0.0f); });
}

static void BM_Vec3Magnitude(benchmark::State& state)
{

    std::vector<neko::Vec3f> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);
    for (auto _ : state)
    {
        for (auto v : local_numbers)
        {
            float f = v.Magnitude();
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
        for (auto v : local_numbers)
        {
            float f = std::sqrt(neko::Vec4f::Dot3(v, v));
            benchmark::DoNotOptimize(f);
        }
    }

}

BENCHMARK(BM_Vec4Magnitude)->Range(fromRange, toRange);

static void BM_Vec3MagnitudeAoSoA4(benchmark::State& state)
{
    std::vector<neko::Vec3f> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);

    for (auto _ : state)
    {
        for (size_t i = 0; i < local_numbers.size(); i += 4)
        {
            const neko::FourVec4f tmp = neko::FourVec4f(&local_numbers[i]);
            benchmark::DoNotOptimize(tmp.Magnitude());
        }
    }
    float result = 0.0f;
    for (size_t i = 0; i < local_numbers.size(); i += 4)
    {
        const neko::FourVec4f tmp = neko::FourVec4f(&local_numbers[i]);
        const auto magnitudes = tmp.Magnitude();
        for (int j = 0; j < 4; j++)
        {
            result += magnitudes[j] - local_numbers[i + j].Magnitude();
        }
    }
    std::cout << "Magnitude difference: " << result / static_cast<float>(state.range(0)) << " Range: "
              << state.range(0) << "\n";

}

BENCHMARK(BM_Vec3MagnitudeAoSoA4)->Range(fromRange, toRange);

static void BM_Vec4MagnitudeAoSoA4(benchmark::State& state)
{
    std::vector<neko::Vec4f> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);

    for (auto _ : state)
    {
        for (size_t i = 0; i < local_numbers.size(); i += 4)
        {
            const neko::FourVec4f tmp = neko::FourVec4f(&local_numbers[i]);
            benchmark::DoNotOptimize(tmp.Magnitude());
        }
    }
    float result = 0.0f;
    for (size_t i = 0; i < local_numbers.size(); i += 4)
    {
        const neko::FourVec4f tmp = neko::FourVec4f(&local_numbers[i]);
        const auto magnitudes = tmp.Magnitude();
        for (int j = 0; j < 4; j++)
        {
            result += magnitudes[j] - local_numbers[i + j].Magnitude();
        }
    }
    std::cout << "Magnitude difference: " << result / static_cast<float>(state.range(0)) << " Range: "
              << state.range(0) << "\n";

}

BENCHMARK(BM_Vec4MagnitudeAoSoA4)->Range(fromRange, toRange);

static void BM_Vec4MagnitudeAoSoA4Intrinsincs(benchmark::State& state)
{
    std::vector<neko::Vec4f> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);
    for (auto _ : state)
    {
        for (size_t i = 0; i < local_numbers.size(); i += 4)
        {
            const neko::FourVec4f tmp = neko::FourVec4f(&local_numbers[i]);
            benchmark::DoNotOptimize(tmp.MagnitudeIntrinsics());
        }
    }
    float result = 0.0f;
    for (size_t i = 0; i < local_numbers.size(); i += 4)
    {
        const neko::FourVec4f tmp = neko::FourVec4f(&local_numbers[i]);
        const auto magnitudes = tmp.Magnitude();
        for (int j = 0; j < 4; j++)
        {
            result += magnitudes[j] - local_numbers[i + j].Magnitude();
        }
    }
    std::cout << "Magnitude difference: " << result / static_cast<float>(state.range(0)) << " Range: "
              << state.range(0) << "\n";

}

BENCHMARK(BM_Vec4MagnitudeAoSoA4Intrinsincs)->Range(fromRange, toRange);

static void BM_Vec3MagnitudeAoSoA8(benchmark::State& state)
{
    std::vector<neko::Vec3f> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);

    for (auto _ : state)
    {
        for (size_t i = 0; i < local_numbers.size(); i += 8)
        {
            const neko::EightVec4f tmp = neko::EightVec4f(&local_numbers[i]);
            benchmark::DoNotOptimize(tmp.Magnitude());
        }
    }
    float result = 0.0f;
    for (size_t i = 0; i < local_numbers.size(); i += 4)
    {
        const neko::FourVec4f tmp = neko::FourVec4f(&local_numbers[i]);
        const auto magnitudes = tmp.Magnitude();
        for (int j = 0; j < 4; j++)
        {
            result += magnitudes[j] - local_numbers[i + j].Magnitude();
        }
    }
    std::cout << "Magnitude difference: " << result / static_cast<float>(state.range(0)) << " Range: "
              << state.range(0) << "\n";

}

BENCHMARK(BM_Vec3MagnitudeAoSoA8)->Range(fromRange, toRange);

static void BM_Vec4MagnitudeAoSoA8(benchmark::State& state)
{
    std::vector<neko::Vec4f> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);

    for (auto _ : state)
    {
        for (size_t i = 0; i < local_numbers.size(); i += 8)
        {
            const neko::EightVec4f tmp = neko::EightVec4f(&local_numbers[i]);
            benchmark::DoNotOptimize(tmp.Magnitude());
        }
    }
    float result = 0.0f;
    for (size_t i = 0; i < local_numbers.size(); i += 4)
    {
        const neko::FourVec4f tmp = neko::FourVec4f(&local_numbers[i]);
        const auto magnitudes = tmp.Magnitude();
        for (int j = 0; j < 4; j++)
        {
            result += magnitudes[j] - local_numbers[i + j].Magnitude();
        }
    }
    std::cout << "Magnitude difference: " << result / static_cast<float>(state.range(0)) << " Range: "
              << state.range(0) << "\n";

}

BENCHMARK(BM_Vec4MagnitudeAoSoA8)->Range(fromRange, toRange);
/*
static void BM_Vec4MagnitudeAoSoA8Intrinsincs(benchmark::State& state)
{
    std::vector<neko::Vec4f> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);

    for (auto _ : state)
    {
        for (size_t i = 0; i < local_numbers.size(); i += 8)
        {
            const neko::EightVec4f tmp = neko::EightVec4f(&local_numbers[i]);
            benchmark::DoNotOptimize(tmp.MagnitudeIntrinsics());
        }
    }

    float result = 0.0f;
    for (size_t i = 0; i < local_numbers.size(); i += 4)
    {
        const neko::FourVec4f tmp = neko::FourVec4f(&local_numbers[i]);
        const auto magnitudes = tmp.Magnitude();
        for (int j = 0; j < 4; j++)
        {
            result += magnitudes[j] - local_numbers[i + j].Magnitude();
        }
    }
    std::cout << "Magnitude difference: " << result / static_cast<float>(state.range(0)) << " Range: "
              << state.range(0) << "\n";

}

BENCHMARK(BM_Vec4MagnitudeAoSoA8Intrinsincs)->Range(fromRange, toRange);
*/
int main(int argc, char** argv)
{
    /*
    std::cout << "__STDCPP_DEFAULT_NEW_ALIGNMENT__ is "
              << __STDCPP_DEFAULT_NEW_ALIGNMENT__ << std::endl;

    std::cout << "sizeof(Vec3f) is " << sizeof(neko::Vec3f) << '\n';
    std::cout << "alignof(Vec3f) is " << alignof(neko::Vec3f) << '\n';
    std::cout << "sizeof(Vec4f) is " << sizeof(neko::Vec4f) << '\n';
    std::cout << "alignof(Vec4f) is " << alignof(neko::Vec4f) << '\n';
    */
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}
