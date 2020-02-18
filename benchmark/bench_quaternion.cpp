//
// Created by efarhan on 21/01/2020.
//
#include <iostream>
#include <random>
#include "random_fill.h"
#include <benchmark/benchmark.h>
#include "mathematics/quaternion.h"

const long fromRange = 2;
const long toRange = 512;
const size_t arraySize = 1024;

/*static void BM_Dot(benchmark::State& state)
{
    std::vector<neko::Quaternion> local_quaternion(arraySize);
	//RandomFill(local_quaternion);
    neko::Quaternion q2 = neko::Quaternion(state.range(0));
	for (auto _ : state)
	{
		for(auto q1 : local_quaternion)
		{
            benchmark::DoNotOptimize(neko::Quaternion::Dot(q1, q2));
		}
	}
}
BENCHMARK(BM_Dot)->Range(2)->Range(fromRange, toRange);*/

/*static void BM_PowRange(benchmark::State& state)
{
    const size_t n = state.range(0);
    std::vector<neko::Mat4f> v1(n, neko::Mat4f::Identity);
    std::vector<neko::Mat4f> v2(n, neko::Mat4f::Identity);

    std::for_each(v1.begin(), v1.end(), [](neko::Mat4f& m) { RandomFill(m); });
    std::for_each(v2.begin(), v2.end(), [](neko::Mat4f& m) { RandomFill(m); });
    for (auto _ : state)
    {
        for (size_t i = 0; i < n; i++)
        {
            benchmark::DoNotOptimize(v1[i].MultiplyNaive(v2[i]));
        }
    }
}
BENCHMARK(BM_PowRange)->RangeMultiplier(2)->Range(fromRange, toRange);*/