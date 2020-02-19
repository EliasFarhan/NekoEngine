//
// Created by efarhan on 21/01/2020.
//
#include <iostream>
#include <random>
#include "random_fill.h"
#include <benchmark/benchmark.h>
#include "mathematics/quaternion.h"

const long fromRange = 2;
const long toRange = 2048;
const size_t arraySize = 1024;

static void BM_Dot(benchmark::State& state)
{
    neko::Quaternion q;
    neko::Vec4f i;
    RandomFill(i);
    q = neko::Quaternion(i.x, i.y, i.z, i.w);
	
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
            neko::Quaternion::Dot(q, element);
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
            neko::Quaternion::Magnitude(element);
        }
    }
}
BENCHMARK(BM_Magnitude)->Range(fromRange, toRange);

static void BM_Angle(benchmark::State& state)
{
    neko::Quaternion q;
    neko::Vec4f i;
    RandomFill(i);
    q = neko::Quaternion(i.x, i.y, i.z, i.w);

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
            neko::Quaternion::Angle(q, element);
        }
    }
}
BENCHMARK(BM_Angle)->Range(fromRange, toRange);

/*static void BM_FromEuler(benchmark::State& state)
{

}
BENCHMARK(BM_FromEuler)->Range(fromRange, toRange);*/