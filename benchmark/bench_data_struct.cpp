#include <benchmark/benchmark.h>
#include <vector>
#include <list>

const unsigned long fromRange = 8;
const unsigned long toRange = 1 << 20;


static void BM_Vector(benchmark::State& state)
{
	const auto length = state.range(0);
	std::vector<int> values(length);
	for (auto& v : values)
	{
		v = rand() % 32768;
	}
	for (auto _ : state)
	{
		for(auto& v : values)
		{
			benchmark::DoNotOptimize(v);
		}
	}
}
BENCHMARK(BM_Vector)->Range(fromRange, toRange);


static void BM_List(benchmark::State& state)
{
	const auto length = state.range(0);
	std::list<int> values;
	for (int i = 0; i < length;i++)
	{
		values.push_back(rand() % 32768);
	}
	for (auto _ : state)
	{
		for (auto& v : values)
		{
			benchmark::DoNotOptimize(v);
		}
	}
}
BENCHMARK(BM_List)->Range(fromRange, toRange);
BENCHMARK_MAIN();