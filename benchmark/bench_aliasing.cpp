#include <benchmark/benchmark.h>
#include <cstdlib>

const unsigned long fromRange = 8;
const unsigned long toRange = 1 << 20;

struct AdderAlias
{
	int mTotal = 0;
	void add(const int* values, const size_t count);
};

void AdderAlias::add(const int* values, const size_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		mTotal += values[i];
	}
}

struct AdderNoAlias
{
	int mTotal = 0;
	void add(const int* values, const size_t count);
};

void AdderNoAlias::add(const int* values, const size_t count)
{
	int total = 0;
	for (size_t i = 0; i < count; i++)
	{
		total += values[i];
	}
	mTotal = total;
}


static void BM_AdderAlias(benchmark::State& state)
{
	const auto length = state.range(0);
	AdderAlias adder;
	std::vector<int> values;
	values.resize(length);
	for(auto& v : values)
	{
		v = rand() % 32768;
	}
	for (auto _ : state)
	{
		adder.mTotal = 0;
		adder.add(&values[0], values.size());
		benchmark::DoNotOptimize(adder);
	}
}
BENCHMARK(BM_AdderAlias)->Range(fromRange, toRange);

static void BM_AdderNoAlias(benchmark::State& state)
{
	const auto length = state.range(0);
	AdderNoAlias adder;
	std::vector<int> values;
	values.resize(length);
	for (auto& v : values)
	{
		v = rand() % 32768;
	}
	for (auto _ : state)
	{
		adder.mTotal = 0;
		adder.add(&values[0], values.size());
		benchmark::DoNotOptimize(adder);
	}
}
BENCHMARK(BM_AdderNoAlias)->Range(fromRange, toRange);
BENCHMARK_MAIN();