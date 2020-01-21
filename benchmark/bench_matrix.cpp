//
// Created by efarhan on 21/01/2020.
//
#include <random>
#include "random_fill.h"
#include <benchmark/benchmark.h>
#include "mathematics/matrix.h"

const long fromRange = 8;
const long toRange = 1<<10;

static void BM_MatrixMultNaive(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Mat4f> v1(n, neko::Mat4f::Identity);
    std::vector<neko::Mat4f> v2(n, neko::Mat4f::Identity);

    std::for_each(v1.begin(), v1.end(), [](neko::Mat4f& m){ RandomFill(m); });
    std::for_each(v2.begin(), v2.end(), [](neko::Mat4f& m){ RandomFill(m); });
    for (auto _ : state)
    {
        for(size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].MultiplyNaive(v2[i]));
        }
    }
}

BENCHMARK(BM_MatrixMultNaive)->Range(fromRange, toRange);
static void BM_MatrixMultTranspose(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Mat4f> v1(n, neko::Mat4f::Identity);
    std::vector<neko::Mat4f> v2(n, neko::Mat4f::Identity);

    std::for_each(v1.begin(), v1.end(), [](neko::Mat4f& m){ RandomFill(m); });
    std::for_each(v2.begin(), v2.end(), [](neko::Mat4f& m){ RandomFill(m); });
    for (auto _ : state)
    {
        for(size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].MultiplyTranpose(v2[i]));
        }
    }
}

BENCHMARK(BM_MatrixMultTranspose)->Range(fromRange, toRange);
static void BM_MatrixMultAoSoA(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Mat4f> v1(n, neko::Mat4f::Identity);
    std::vector<neko::Mat4f> v2(n, neko::Mat4f::Identity);

    std::for_each(v1.begin(), v1.end(), [](neko::Mat4f& m){ RandomFill(m); });
    std::for_each(v2.begin(), v2.end(), [](neko::Mat4f& m){ RandomFill(m); });
    for (auto _ : state)
    {
        for(size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].MultiplyAoSoA(v2[i]));
        }
    }
}

BENCHMARK(BM_MatrixMultAoSoA)->Range(fromRange, toRange);



static void BM_MatrixMultIntrinsics(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Mat4f> v1(n, neko::Mat4f::Identity);
    std::vector<neko::Mat4f> v2(n, neko::Mat4f::Identity);

    std::for_each(v1.begin(), v1.end(), [](neko::Mat4f& m){ RandomFill(m); });
    std::for_each(v2.begin(), v2.end(), [](neko::Mat4f& m){ RandomFill(m); });
    for (auto _ : state)
    {
        for(size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].MultiplyIntrinsincs(v2[i]));
        }

    }
}

BENCHMARK(BM_MatrixMultIntrinsics)->Range(fromRange, toRange);