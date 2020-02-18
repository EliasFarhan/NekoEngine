#include <benchmark/benchmark.h>
#include <mathematics/vector_nvec.h>

static void BM_SquareMagnitude(benchmark::State& state)
{
    std::array<neko::Vec4f, 8> array;

    for (auto _ : state)
    {
        for (auto& arr : array)
        {
            arr = neko::Vec4f(rand(), rand(), rand(), rand());
        }

        neko::EightVec4f test = (neko::EightVec4f) array;

        test.SquareMagnitude();
    }
}

BENCHMARK(BM_SquareMagnitude);

static void BM_SquareMagnitudeIntrinsics(benchmark::State& state)
{
    std::array<neko::Vec4f, 8> array;

    for (auto _ : state)
    {
        for (auto& arr : array)
        {
            arr = neko::Vec4f(rand(), rand(), rand(), rand());
        }

        neko::EightVec4f test = (neko::EightVec4f) array;

        test.SquareMagnitudeIntrinsics();
    }
}

BENCHMARK(BM_SquareMagnitudeIntrinsics);

static void BM_Magnitude(benchmark::State& state)
{
    std::array<neko::Vec4f, 8> array;

    for (auto _ : state)
    {
        for (auto& arr : array)
        {
            arr = neko::Vec4f(rand(), rand(), rand(), rand());
        }

        neko::EightVec4f test = (neko::EightVec4f) array;

        test.Magnitude();
    }
}

BENCHMARK(BM_Magnitude);

static void BM_MagnitudeIntrinsics(benchmark::State& state)
{
    std::array<neko::Vec4f, 8> array;

    for (auto _ : state)
    {
        for (auto& arr : array)
        {
            arr = neko::Vec4f(rand(), rand(), rand(), rand());
        }

        neko::EightVec4f test = (neko::EightVec4f) array;

        test.MagnitudeIntrinsics();
    }
}

BENCHMARK(BM_MagnitudeIntrinsics);

BENCHMARK_MAIN();
