#include <benchmark/benchmark.h>

#include <atomic>
#include "ctpl_stl.h"
#include <array>

const int fromRange = 8;
const int toRange = 5000;

void add_n(int& value, int n)
{
	for (int i = 0; i < n; i++)
	{
		++value;
	}
}

void add_atomic_n(int id, std::atomic<int>& value, int n)
{
	for (int i = 0; i < n; i++)
	{
		++value;
	}
}



static void BM_IntSum(benchmark::State& state)
{

	int a = 0;
	for (auto _ : state)
	{
		a = 0;
		add_n(a, state.range(0));
	}
}

BENCHMARK(BM_IntSum)->Range(fromRange, toRange);
static void BM_AtomicIntSum(benchmark::State& state)
{

	std::atomic<int> a = 0;
	for (auto _ : state)
	{
		a = 0;
		add_atomic_n(0, a, state.range(0));
	}
}

BENCHMARK(BM_AtomicIntSum)->Range(fromRange, toRange);

static void BM_ThreadAtomic2IntSum(benchmark::State& state)
{
	const size_t n = 2;
	ctpl::thread_pool tp(n);
	std::array<std::future<void>, n> returnAdd;
	std::array<std::thread, n> threads;
	std::atomic<int> a = 0;
	for (auto _ : state)
	{
		a = 0;
		for(size_t i = 0; i < n; i++)
		{
			returnAdd[i] = tp.push(add_atomic_n, std::ref(a), int(state.range(0) / n));
		}
		std::for_each(returnAdd.begin(), returnAdd.end(), [](std::future<void>& f) {f.get(); });
	}
}

BENCHMARK(BM_ThreadAtomic2IntSum)->Range(fromRange, toRange);

static void BM_ThreadAtomic4IntSum(benchmark::State& state)
{
	const size_t n = 4;
	ctpl::thread_pool tp(n);
	std::array<std::future<void>, n> returnAdd;
	std::array<std::thread, n> threads;
	std::atomic<int> a = 0;
	for (auto _ : state)
	{
		a = 0;
		for (size_t i = 0; i < n; i++)
		{
			returnAdd[i] = tp.push(add_atomic_n, std::ref(a), int(state.range(0) / n));
		}
		std::for_each(returnAdd.begin(), returnAdd.end(), [](std::future<void>& f) {f.get(); });
	}
}

BENCHMARK(BM_ThreadAtomic4IntSum)->Range(fromRange, toRange);