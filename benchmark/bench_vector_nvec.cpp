#include <benchmark/benchmark.h>
#include <mathematics/vector_nvec.h>

static void BM_SquareMagnitudeSimple(benchmark::State& state)
{
    std::array<neko::Vec4f, 8> array;
    for (auto& arr : array)
    {
        arr = neko::Vec4f(5, 5, 5, 5);
    }

    for (auto _ : state)
    {
        for (auto& vec : array)
        {
            vec.SquareMagnitude();
        }
    }
}

BENCHMARK(BM_SquareMagnitudeSimple);

static void BM_SquareMagnitude(benchmark::State& state)
{
    std::array<neko::Vec4f, 8> array;
    for (auto& arr : array)
    {
        arr = neko::Vec4f(5, 5, 5, 5);
    }

    neko::EightVec4f test = (neko::EightVec4f) array;

    for (auto _ : state)
    {
        test.SquareMagnitude();
    }
}

BENCHMARK(BM_SquareMagnitude);
/*
static void BM_SquareMagnitudeIntrinsics(benchmark::State& state)
{
    std::array<neko::Vec4f, 8> array;
    for (auto& arr : array)
    {
        arr = neko::Vec4f(5, 5, 5, 5);
    }

    neko::EightVec4f test = (neko::EightVec4f) array;

    for (auto _ : state)
    {
        test.SquareMagnitudeIntrinsics();
    }
}


BENCHMARK(BM_SquareMagnitudeIntrinsics);
*/
static void BM_MagnitudeSimple(benchmark::State& state)
{
    std::array<neko::Vec4f, 8> array;
    for (auto& arr : array)
    {
        arr = neko::Vec4f(5, 5, 5, 5);
    }

    for (auto _ : state)
    {
        for (auto& vec : array)
        {
            vec.Magnitude();
        }
    }
}

BENCHMARK(BM_MagnitudeSimple);

static void BM_Magnitude(benchmark::State& state)
{
    std::array<neko::Vec4f, 8> array;
    for (auto& arr : array)
    {
        arr = neko::Vec4f(5, 5, 5, 5);
    }

    neko::EightVec4f test = (neko::EightVec4f) array;

    for (auto _ : state)
    {
        test.Magnitude();
    }
}

BENCHMARK(BM_Magnitude);
/*
static void BM_MagnitudeIntrinsics(benchmark::State& state)
{
    std::array<neko::Vec4f, 8> array;
    for (auto& arr : array)
    {
        arr = neko::Vec4f(5, 5, 5, 5);
    }

    neko::EightVec4f test = (neko::EightVec4f) array;


    for (auto _ : state)
    {
        test.MagnitudeIntrinsics();
    }
}

BENCHMARK(BM_MagnitudeIntrinsics);
*/
static void BM_DotSimple(benchmark::State& state)
{
    std::array<neko::Vec4f, 8> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec4f(5, 5, 5, 5);
    }

    std::array<neko::Vec4f, 8> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec4f(5, 5, 5, 5);
    }

    for (auto _ : state)
    {
        for (int i = 0; i < 8; ++i)
        {
            neko::Vec4f::Dot(array1[i], array2[i]);
        }
    }
}

BENCHMARK(BM_DotSimple);

static void BM_Dot(benchmark::State& state)
{
    std::array<neko::Vec4f, 8> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec4f(5, 5, 5, 5);
    }
    neko::EightVec4f test1 = (neko::EightVec4f) array1;

    std::array<neko::Vec4f, 8> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec4f(5, 5, 5, 5);
    }
    neko::EightVec4f test2 = (neko::EightVec4f) array2;

    for (auto _ : state)
    {
        neko::EightVec4f::Dot(test1, test2);
    }
}

BENCHMARK(BM_Dot);
/*
static void BM_DotIntrinsics(benchmark::State& state)
{
    std::array<neko::Vec4f, 8> array1;
    for (auto& arr : array1)
    {
        arr = neko::Vec4f(5, 5, 5, 5);
    }
    neko::EightVec4f test1 = (neko::EightVec4f) array1;

    std::array<neko::Vec4f, 8> array2;
    for (auto& arr : array2)
    {
        arr = neko::Vec4f(5, 5, 5, 5);
    }
    neko::EightVec4f test2 = (neko::EightVec4f) array2;

    for (auto _ : state)
    {
        neko::EightVec4f::DotIntrinsics(test1, test2);
    }
}

BENCHMARK(BM_DotIntrinsics);
*/
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
        for (int i = 0; i < 8; ++i)
        {
            neko::Vec3f::Reflect(array1[i], array2[i]);
        }
    }
}

BENCHMARK(BM_ReflectSimple);

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
        neko::EightVec3f::Reflect(test1, test2);
    }
}

BENCHMARK(BM_Reflect);
/*
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
        neko::EightVec3f::ReflectIntrinsics(test1, test2);
    }
}

BENCHMARK(BM_ReflectIntrinsics);
*/
BENCHMARK_MAIN();
