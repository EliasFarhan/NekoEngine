#ifdef WIN32
#define __SSE__
#include <iostream>
#include <random>
#include "random_fill.h"
#include <benchmark/benchmark.h>
#include <mathematics/quaternion.h>

const long fromRange = 2;
const long toRange = 2048;
const size_t arraySize = 1024;

/*
 * static void BM_Normalized(benchmark::State& state)
{
    for (auto s : state)
    {
        for (int i = 0; i < state.range(); i++)
        {
            benchmark::DoNotOptimize();
        }
    }
}
BENCHMARK(BM_Normalized)->Range(fromRange, toRange);
 */

/*static void BM_Quaternion(benchmark::State& state)
{
    neko::Quaternion q;
    for (auto s : state)
    {
        for (int i = 0; i < state.range(); i++)
        {
            benchmark::DoNotOptimize(q = neko::Quaternion());
        }
    }
}
BENCHMARK(BM_Quaternion)->Range(fromRange, toRange);

static void BM_QuaternionXYZ(benchmark::State& state)
{
    neko::Vec4f v;
    RandomFill(v);
    for (auto s : state)
    {
        for (int i = 0; i < state.range(); i++)
        {
            benchmark::DoNotOptimize(neko::Quaternion(v.x, v.y, v.z, v.w));
        }
    }
}
BENCHMARK(BM_QuaternionXYZ)->Range(fromRange, toRange);

static void BM_Normalized(benchmark::State& state)
{
    neko::Vec4f v;
    RandomFill(v);
    neko::Quaternion q = neko::Quaternion(v.x, v.y, v.z, v.w);
    neko::Quaternion result;
	
    for (auto s : state)
    {
        for (int i = 0; i < state.range(); i++)
        {
            benchmark::DoNotOptimize(result = neko::Quaternion::Normalized(q));
        }
    }
}
BENCHMARK(BM_Normalized)->Range(fromRange, toRange);

static void BM_AngleAxis(benchmark::State& state)
{
    neko::radian_t rad = neko::radian_t(RandomFloat());
	
    neko::Vec3f vec3;
    RandomFill(vec3);
    for (auto s : state)
    {
        for (int i = 0; i < state.range(); i++)
        {
            benchmark::DoNotOptimize(neko::Quaternion::AngleAxis(rad, vec3));
        }
    }
}
BENCHMARK(BM_AngleAxis)->Range(fromRange, toRange);

static void BM_Conjugate(benchmark::State& state)
{
    neko::Vec4f v;
    RandomFill(v);
    neko::Quaternion q = neko::Quaternion(v.x, v.y, v.z, v.w);
    for (auto s : state)
    {
        for (int i = 0; i < state.range(); i++)
        {
            benchmark::DoNotOptimize(q.Conjugate());
        }
    }
}
BENCHMARK(BM_Conjugate)->Range(fromRange, toRange);*/

/*static void BM_Dot(benchmark::State& state)
{
    neko::Quaternion q;
    neko::Vec4f vi;
    RandomFill(vi);
    q = neko::Quaternion(vi.x, vi.y, vi.z, vi.w);
	
    std::vector<neko::Quaternion> vq;
    for (int i = 0; i < state.range(0); i++)
    {
        neko::Vec4f v;
        RandomFill(v);
        vq.push_back(neko::Quaternion(v.x, v.y, v.z, v.w));
    }
	
	for (auto s : state)
	{
		for (neko::Quaternion element : vq)
		{
            benchmark::DoNotOptimize(neko::Quaternion::Dot(q, element));
		}
	}
}
BENCHMARK(BM_Dot)->Range(fromRange, toRange);*/

static void BM_Dot(benchmark::State& state)
{
    const size_t n = state.range(0)*4;
    std::vector<neko::Quaternion> v1(n, neko::Quaternion::Identity());
    std::vector<neko::Quaternion> v2(n, neko::Quaternion::Identity());
    std::for_each(v1.begin(), v1.end(), [](neko::Quaternion& q) {RandomFill(q); });
    std::for_each(v2.begin(), v2.end(), [](neko::Quaternion& q) {RandomFill(q); });

    size_t iterations = 0;
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(neko::Quaternion::Dot(v1[i], v2[i]));
        }
    }
}
BENCHMARK(BM_Dot)->Range(fromRange, toRange);

static void BM_Dot_Intrinsics(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::FourQuaternion> v1(n, neko::FourQuaternion::Identity());
    std::vector<neko::FourQuaternion> v2(n, neko::FourQuaternion::Identity());
    std::for_each(v1.begin(), v1.end(), [](neko::FourQuaternion& q) {RandomFill(q); });
    std::for_each(v2.begin(), v2.end(), [](neko::FourQuaternion& q) {RandomFill(q); });

    size_t iterations = 0;
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(neko::FourQuaternion::Dot(v1[i], v2[i]));
        }
    }
}
BENCHMARK(BM_Dot_Intrinsics)->Range(fromRange, toRange);

/*static void BM_Four_Dot(benchmark::State& state)
{
    //const size_t n = state.range(0);
    //std::vector<neko::Quaternion> qV1(n*4, neko::Quaternion::Identity());
	
    neko::Quaternion q1a = neko::Quaternion(5, 7, 30, 8);
    neko::Quaternion q1b = neko::Quaternion(6, 3, 2, 1);
    neko::Quaternion q1c = neko::Quaternion(42, 666, 69, 13);
    neko::Quaternion q1d = neko::Quaternion(29, 382, 4729, 773);
    neko::Quaternion q2 = neko::Quaternion(7, 3, 1, 9);

    size_t iterations = 0;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(neko::Quaternion::Dot(q1a, q2));
        benchmark::DoNotOptimize(neko::Quaternion::Dot(q1b, q2));
        benchmark::DoNotOptimize(neko::Quaternion::Dot(q1c, q2));
        benchmark::DoNotOptimize(neko::Quaternion::Dot(q1d, q2));
    }
}
BENCHMARK(BM_Four_Dot)->Range(fromRange, toRange);

static void BM_Four_Intrinsics_Dot(benchmark::State& state)
{
    std::array<float, 4> x1{ 5,6,42,29 };
    std::array<float, 4> y1{ 7,3,666,382 };
    std::array<float, 4> z1{ 30,2,69,4729 };
    std::array<float, 4> w1{ 8,1,13,773 };
    std::array<float, 4> x2{ 7,7,7,7 };
    std::array<float, 4> y2{ 3,3,3,3 };
    std::array<float, 4> z2{ 1,1,1,1 };
    std::array<float, 4> w2{ 9,9,9,9 };

    neko::FourQuaternion fqa = neko::FourQuaternion(x1, y1, z1, w1);
    neko::FourQuaternion fqb = neko::FourQuaternion(x2, y2, z2, w2);
    size_t iterations = 0;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(neko::FourQuaternion::DotIntrinsics(fqa, fqb));
    }
}
BENCHMARK(BM_Four_Intrinsics_Dot)->Range(fromRange, toRange);*/

static void BM_Magnitude(benchmark::State& state)
{
    std::vector<neko::Quaternion> vq;
    for (int i = 0; i < state.range(0); i++)
    {
        neko::Vec4f v;
        RandomFill(v);
        vq.push_back(neko::Quaternion(v.x, v.y, v.z, v.w));
    }

    for (auto s : state)
    {
        for (neko::Quaternion element : vq)
        {
            benchmark::DoNotOptimize(neko::Quaternion::Magnitude(element));
        }
    }
}
/*BENCHMARK(BM_Magnitude)->Range(fromRange, toRange);

static void BM_Angle(benchmark::State& state)
{
    neko::Vec4f vi;
    RandomFill(vi);
    neko::Quaternion q1 = neko::Quaternion(vi.x, vi.y, vi.z, vi.w);

    std::vector<neko::Quaternion> vq;
    for (int i = 0; i < state.range(0); i++)
    {
        neko::Vec4f v;
        RandomFill(v);
        vq.push_back(neko::Quaternion(v.x, v.y, v.z, v.w));
    }

    for (auto s : state)
    {
        for (auto& element : vq)
        {
            benchmark::DoNotOptimize(neko::Quaternion::Angle(q1, element));
        }
    }
}
BENCHMARK(BM_Angle)->Range(fromRange, toRange);

static void BM_Inverse(benchmark::State& state)
{
    for (auto s : state)
    {
        neko::Vec4f v;
        RandomFill(v);
        neko::Quaternion q = neko::Quaternion(v.x, v.y, v.z, v.w);
        for (int i = 0; i < state.range(); i++)
        {
            benchmark::DoNotOptimize(q.Inverse());
        }
    }
}
BENCHMARK(BM_Inverse)->Range(fromRange, toRange);

static void BM_FromEuler(benchmark::State& state)
{
    neko::EulerAngles angle = neko::Vec3<neko::degree_t>(neko::degree_t(RandomFloat()));
    neko::Vec3f v3;
    RandomFill(v3);
    for (auto s : state)
    {
        for (int i = 0; i < state.range(); i++)
        {
            benchmark::DoNotOptimize(neko::Quaternion::FromEuler(angle));
        }
    }
}
BENCHMARK(BM_FromEuler)->Range(fromRange, toRange);

static void BM_Identity(benchmark::State& state)
{
    for (auto s : state)
    {
	    neko::Quaternion q;
        for (int i = 0; i < state.range(); i++)
        {
            benchmark::DoNotOptimize(q = neko::Quaternion::Identity());
        }
    }
}
BENCHMARK(BM_Identity)->Range(fromRange, toRange);*/

BENCHMARK_MAIN();
#endif
