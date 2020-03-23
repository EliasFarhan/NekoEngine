#include <iostream>
#include <random>
#include "random_fill.h"
#include <benchmark/benchmark.h>
#include "mathematics/quaternion.h"

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

static void BM_Quaternion(benchmark::State& state)
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
BENCHMARK(BM_Conjugate)->Range(fromRange, toRange);

static void BM_Dot(benchmark::State& state)
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
BENCHMARK(BM_Dot)->Range(fromRange, toRange);

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
BENCHMARK(BM_Magnitude)->Range(fromRange, toRange);

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
BENCHMARK(BM_Identity)->Range(fromRange, toRange);

BENCHMARK_MAIN();