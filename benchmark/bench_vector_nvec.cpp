#include <benchmark/benchmark.h>
#include <mathematics/vector_nvec.h>

const long fromRange = 4;
const long toRange = 1 << 10;

static void BM_SquareMagnitudeSimple(benchmark::State& state)
{
    std::array<neko::Vec3f, 4> array;
    for (auto& arr : array)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
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
    std::array<neko::Vec3f, 4> array;
    for (auto& arr : array)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }

    const neko::FourVec3f fourVec3(array);

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(fourVec3.SquareMagnitude());
        }
    }
}

BENCHMARK(BM_SquareMagnitude)->Range(fromRange, toRange);

static void BM_SquareMagnitudeIntrinsics(benchmark::State& state)
{
    std::array<neko::Vec3f, 4> array;
    for (auto& arr : array)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }

    const neko::FourVec3f fourVec3(array);

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(fourVec3.SquareMagnitudeIntrinsics());
        }
    }
}

BENCHMARK(BM_SquareMagnitudeIntrinsics)->Range(fromRange, toRange);

static void BM_MagnitudeSimple(benchmark::State& state)
{
    std::array<neko::Vec3f, 4> array;
    for (auto& arr : array)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
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
    std::array<neko::Vec3f, 4> array;
    for (auto& arr : array)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }

    const neko::FourVec3f fourVec3(array);

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(fourVec3.Magnitude());
        }
    }
}

BENCHMARK(BM_Magnitude)->Range(fromRange, toRange);

static void BM_MagnitudeIntrinsics(benchmark::State& state)
{
    std::array<neko::Vec3f, 4> array;
    for (auto& arr : array)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }

    const neko::FourVec3f fourVec3(array);


    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(fourVec3.MagnitudeIntrinsics());
        }
    }
}

BENCHMARK(BM_MagnitudeIntrinsics)->Range(fromRange, toRange);

static void BM_MagnitudeIntrinsicsFourVec(benchmark::State& state)
{
    std::array<neko::Vec3f, 4> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }
    const neko::FourVec3f fourVec3(array1);

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(fourVec3.MagnitudeIntrinsics());
        }
    }
}

BENCHMARK(BM_MagnitudeIntrinsicsFourVec)->Range(fromRange, toRange);

static void BM_DotSimple(benchmark::State& state)
{
    std::array<neko::Vec3f, 4> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }

    std::array<neko::Vec3f, 4> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            for (size_t j = 0; j < 4; ++j)
            {
                benchmark::DoNotOptimize(neko::Vec3f::Dot(array1[j], array2[j]));
            }
        }
    }
}

BENCHMARK(BM_DotSimple)->Range(fromRange, toRange);

static void BM_Dot(benchmark::State& state)
{
    std::array<neko::Vec3f, 4> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }
    const neko::FourVec3f fourVec31(array1);

    std::array<neko::Vec3f, 4> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }
    const neko::FourVec3f fourVec32(array2);

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(neko::FourVec3f::Dot(fourVec31, fourVec32));
        }
    }
}

BENCHMARK(BM_Dot)->Range(fromRange, toRange);

static void BM_DotIntrinsics(benchmark::State& state)
{
    std::array<neko::Vec3f, 4> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }
    const neko::FourVec3f fourVec31(array1);

    std::array<neko::Vec3f, 4> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }
    const neko::FourVec3f fourVec32(array2);

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(neko::FourVec3f::DotIntrinsics(fourVec31, fourVec32));
        }
    }
}

BENCHMARK(BM_DotIntrinsics)->Range(fromRange, toRange);

static void BM_DotIntrinsicsEightVec(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }
    const neko::EightVec3f eightVec31(array1);

    std::array<neko::Vec3f, 8> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }
    const neko::EightVec3f eightVec32(array2);

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(neko::EightVec3f::DotIntrinsics(eightVec31, eightVec32));
        }
    }
}

BENCHMARK(BM_DotIntrinsicsEightVec)->Range(fromRange, toRange);

static void BM_ReflectSimple(benchmark::State& state)
{
    std::array<neko::Vec3f, 4> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }

    std::array<neko::Vec3f, 4> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            for (size_t j = 0; j < 4; ++j)
            {
                benchmark::DoNotOptimize(neko::Vec3f::Reflect(array1[j], array2[j]));
            }
        }
    }
}

BENCHMARK(BM_ReflectSimple)->Range(fromRange, toRange);

static void BM_Reflect(benchmark::State& state)
{
    std::array<neko::Vec3f, 4> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }
    const neko::FourVec3f fourVec31(array1);

    std::array<neko::Vec3f, 4> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }
    const neko::FourVec3f fourVec32(array2);

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
			benchmark::DoNotOptimize(neko::FourVec3f::Reflect(fourVec31, fourVec32));
        }
    }
}

BENCHMARK(BM_Reflect)->Range(fromRange, toRange);

static void BM_ReflectIntrinsics(benchmark::State& state)
{
    std::array<neko::Vec3f, 4> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }
    const neko::FourVec3f fourVec31(array1);

    std::array<neko::Vec3f, 4> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }
    const neko::FourVec3f fourVec32(array2);

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(neko::FourVec3f::ReflectIntrinsics(fourVec31, fourVec32));
        }
    }
}

BENCHMARK(BM_ReflectIntrinsics)->Range(fromRange, toRange);

static void BM_ReflectIntrinsicsEightVec(benchmark::State& state)
{
    std::array<neko::Vec3f, 8> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }
    const neko::EightVec3f eightVec31(array1);

    std::array<neko::Vec3f, 8> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec3f(std::rand(), std::rand(), std::rand());
    }
    const neko::EightVec3f eightVec32(array2);

    for (auto _ : state)
    {
        for (size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(neko::EightVec3f::ReflectIntrinsics(eightVec31, eightVec32));
        }
    }
}

BENCHMARK(BM_ReflectIntrinsicsEightVec)->Range(fromRange, toRange);

BENCHMARK_MAIN();
