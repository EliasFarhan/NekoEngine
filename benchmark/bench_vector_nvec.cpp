#include <benchmark/benchmark.h>
#include <mathematics/vector_nvec.h>

const long fromRange = 8;
const long toRange = 1 << 10;

static void BM_SquareMagnitudeSimple(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array;
    for (auto& arr : array)
    {
        arr = neko::Vec3f(5, 5, 5);
    }

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            for (auto& vec : array)
            {
                benchmark::DoNotOptimize(vec.SquareMagnitude());
            }
        }
    }
}

BENCHMARK(BM_SquareMagnitudeSimple)->Range(fromRange, toRange);

static void BM_SquareMagnitude(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array;
    for (auto& arr : array)
    {
        arr = neko::Vec3f(5, 5, 5);
    }

    neko::EightVec3f test = (neko::EightVec3f) array;

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(test.SquareMagnitude());
        }
    }
}

BENCHMARK(BM_SquareMagnitude)->Range(fromRange, toRange);

static void BM_SquareMagnitudeIntrinsics(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array;
    for (auto& arr : array)
    {
        arr = neko::Vec3f(5, 5, 5);
    }

    neko::EightVec3f test = (neko::EightVec3f) array;

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(test.SquareMagnitudeIntrinsics());
        }
    }
}

BENCHMARK(BM_SquareMagnitudeIntrinsics)->Range(fromRange, toRange);

static void BM_MagnitudeSimple(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array;
    for (auto& arr : array)
    {
        arr = neko::Vec3f(5, 5, 5);
    }

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            for (auto& vec : array)
            {
                benchmark::DoNotOptimize(vec.Magnitude());
            }
        }
    }
}

BENCHMARK(BM_MagnitudeSimple)->Range(fromRange, toRange);

static void BM_Magnitude(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array;
    for (auto& arr : array)
    {
        arr = neko::Vec3f(5, 5, 5);
    }

    neko::EightVec3f test = (neko::EightVec3f) array;

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(test.Magnitude());
        }
    }
}

BENCHMARK(BM_Magnitude)->Range(fromRange, toRange);

static void BM_MagnitudeIntrinsics(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array;
    for (auto& arr : array)
    {
        arr = neko::Vec3f(5, 5, 5);
    }

    neko::EightVec3f test = (neko::EightVec3f) array;


    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(test.MagnitudeIntrinsics());
        }
    }
}

BENCHMARK(BM_MagnitudeIntrinsics)->Range(fromRange, toRange);

static void BM_DotSimple(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(5, 5, 5);
    }

    std::array<neko::Vec3f, 8> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(5, 5, 5);
    }

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            for (size_t i = 0; i < 8; ++i)
            {
                benchmark::DoNotOptimize(neko::Vec3f::Dot(array1[i], array2[i]));
            }
        }
    }
}

BENCHMARK(BM_DotSimple)->Range(fromRange, toRange);

static void BM_Dot(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(5, 5, 5);
    }
    neko::EightVec3f test1 = (neko::EightVec3f) array1;

    std::array<neko::Vec3f, 8> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(5, 5, 5);
    }
    neko::EightVec3f test2 = (neko::EightVec3f) array2;

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(neko::EightVec3f::Dot(test1, test2));
        }
    }
}

BENCHMARK(BM_Dot)->Range(fromRange, toRange);

static void BM_DotIntrinsics(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(5, 5, 5);
    }
    neko::EightVec3f test1 = (neko::EightVec3f) array1;

    std::array<neko::Vec3f, 8> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(5, 5, 5);
    }
    neko::EightVec3f test2 = (neko::EightVec3f) array2;

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(neko::EightVec3f::DotIntrinsics(test1, test2));
        }
    }
}

BENCHMARK(BM_DotIntrinsics)->Range(fromRange, toRange);

static void BM_ReflectSimple(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(1, 1, 0);
    }

    std::array<neko::Vec3f, 8> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(0, -1, 0);
    }

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            for (size_t i = 0; i < 8; ++i)
            {
                benchmark::DoNotOptimize(neko::Vec3f::Reflect(array1[i], array2[i]));
            }
        }
    }
}

BENCHMARK(BM_ReflectSimple)->Range(fromRange, toRange);

static void BM_Reflect(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(1, 1, 0);
    }
    neko::EightVec3f test1 = (neko::EightVec3f) array1;

    std::array<neko::Vec3f, 8> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(0, -1, 0);
    }
    neko::EightVec3f test2 = (neko::EightVec3f) array2;

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
			benchmark::DoNotOptimize(neko::EightVec3f::Reflect(test1, test2));
        }
    }
}

BENCHMARK(BM_Reflect)->Range(fromRange, toRange);

static void BM_ReflectIntrinsics(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(1, 1, 0);
    }
    neko::EightVec3f test1 = (neko::EightVec3f) array1;

    std::array<neko::Vec3f, 8> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(0, -5, 0);
    }
    neko::EightVec3f test2 = (neko::EightVec3f) array2;

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(neko::EightVec3f::ReflectIntrinsics(test1, test2));
        }
    }
}

BENCHMARK(BM_ReflectIntrinsics)->Range(fromRange, toRange);

BENCHMARK_MAIN();
