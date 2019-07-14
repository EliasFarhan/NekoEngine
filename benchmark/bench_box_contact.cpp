#include "benchmark/benchmark.h"
#include "gb/box_collision.h"


const unsigned long fromRange = 8;
const unsigned long toRange = 32768;

#define BOX_LENGTH 12
Box boxes[BOX_LENGTH] =
{
	{2,2,3,3},
	{0,0,3,3},
	{3,0,1,3},
	{4,0,3,3},
	{0,3,3,1},
	{4,3,3,1},
	{0,4,3,3},
	{3,4,1,3},
	{4,4,3,3},
	{0,3,7,1},
	{3,0,1,7},
	{0,0,2,2}
};


static void BM_Box1(benchmark::State& state)
{
	const auto length = state.range(0);
	for (auto _ : state)
	{
		for (auto k = decltype(length)(0); k < length; k++)
		{
			for (int i = 1; i < BOX_LENGTH; i++)
			{
				benchmark::DoNotOptimize(checkCollision1(&boxes[0], &boxes[i]));
			}
		}
	}
}
BENCHMARK(BM_Box1)->Range(fromRange, toRange);
static void BM_Box2(benchmark::State& state)
{
	const unsigned long length = state.range(0);
	for (auto k = decltype(length)(0); k < length; k++)
	{
		for (auto _ : state)
		{
			for (int i = 1; i < BOX_LENGTH; i++)
			{
				benchmark::DoNotOptimize(checkCollision2(&boxes[0], &boxes[i]));
			}
		}
	}
}
BENCHMARK(BM_Box2)->Range(fromRange, toRange);
static void BM_Box3(benchmark::State& state)
{
	const unsigned long length = state.range(0);
	for (auto k = decltype(length)(0); k < length; k++)
	{
		for (auto _ : state)
		{
			for (int i = 1; i < BOX_LENGTH; i++)
			{
				benchmark::DoNotOptimize(checkCollision3(&boxes[0], &boxes[i]));
			}
		}
	}
}
BENCHMARK(BM_Box3)->Range(fromRange, toRange);

BENCHMARK_MAIN();