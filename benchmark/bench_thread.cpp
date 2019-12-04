#include <benchmark/benchmark.h>

#include <atomic>
#include "ctpl_stl.h"
#include <array>
#include <cassert>
#include <algorithm>
#include <numeric>

const int fromRange = 8;
const int toRange = 5000;

void add_n_lockaround(int id, int& value, int n)
{
    static std::mutex m;
    m.lock();
	for (int i = 0; i < n; i++)
	{
		++value;
	}
	m.unlock();
}

void add_n_lockin(int id, int& value, int n)
{
    static std::mutex m;
    for (int i = 0; i < n; i++)
    {
        m.lock();
        ++value;
        m.unlock();
    }
}

void add_atomic_n(int id, std::atomic<int>& value, int n)
{
	for (int i = 0; i < n; i++)
	{
		++value;
	}
}



static void BM_IntSumMutexAround(benchmark::State& state)
{

	int a = 0;
	for (auto _ : state)
	{
		a = 0;
        add_n_lockaround(0, a, state.range(0));
		benchmark::DoNotOptimize(a);
        assert(a == state.range(0));
	}
}

BENCHMARK(BM_IntSumMutexAround)->Range(fromRange, toRange)->UseRealTime();

static void BM_IntSumMutexIn(benchmark::State& state)
{

    int a = 0;
    for (auto _ : state)
    {
        a = 0;
        add_n_lockin(0, a, state.range(0));
        benchmark::DoNotOptimize(a);
        assert(a == state.range(0));
    }
}

BENCHMARK(BM_IntSumMutexIn)->Range(fromRange, toRange)->UseRealTime();

static void BM_AtomicIntSum(benchmark::State& state)
{

	std::atomic<int> a = 0;
	for (auto _ : state)
	{
		a = 0;
		add_atomic_n(0, a, state.range(0));
        assert(a == state.range(0));
	}
}

BENCHMARK(BM_AtomicIntSum)->Range(fromRange, toRange)->UseRealTime();

static void BM_ThreadAtomic2IntSum(benchmark::State& state)
{
	const size_t n = 2;
	ctpl::thread_pool tp(n);
	std::array<std::future<void>, n> returnAdd;
	std::atomic<int> a = 0;
	for (auto _ : state)
	{
		a = 0;
		for(size_t i = 0; i < n; i++)
		{
			returnAdd[i] = tp.push(add_atomic_n, std::ref(a), int(state.range(0) / n));
		}
		std::for_each(returnAdd.begin(), returnAdd.end(), [](std::future<void>& f) { f.get(); });
        assert(a == state.range(0));
	}
}

BENCHMARK(BM_ThreadAtomic2IntSum)->Range(fromRange, toRange)->UseRealTime()->Threads(2);

static void BM_ThreadAtomic4IntSum(benchmark::State& state)
{
	const size_t n = 4;
	ctpl::thread_pool tp(n);
	std::array<std::future<void>, n> returnAdd;
	std::atomic<int> a = 0;
	for (auto _ : state)
	{
		a = 0;
		for (size_t i = 0; i < n; i++)
		{
			returnAdd[i] = tp.push(add_atomic_n, std::ref(a), int(state.range(0) / n));
		}
		std::for_each(returnAdd.begin(), returnAdd.end(), [](std::future<void>& f) { f.get(); });
		assert(a == state.range(0));
	}
}

BENCHMARK(BM_ThreadAtomic4IntSum)->Range(fromRange, toRange)->UseRealTime()->Threads(4);

static void BM_ThreadAtomic2SeparateIntSum(benchmark::State& state)
{
    const size_t n = 2;
    ctpl::thread_pool tp(n);
    std::array<std::future<void>, n> returnAdd;
    std::array<std::atomic<int>, n> aArray;

    for (auto _ : state)
    {
        std::fill(aArray.begin(), aArray.end(), 0);
        for(size_t i = 0; i < n; i++)
        {
            returnAdd[i] = tp.push(add_atomic_n, std::ref(aArray[i]), int(state.range(0) / n));
        }
        std::for_each(returnAdd.begin(), returnAdd.end(), [](std::future<void>& f) { f.get(); });
        int result = std::accumulate(aArray.begin(), aArray.end(), 0);
        assert(result == state.range(0));

        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ThreadAtomic2SeparateIntSum)->Range(fromRange, toRange)->UseRealTime()->Threads(2);

static void BM_ThreadAtomic4SeparateIntSum(benchmark::State& state)
{
    const size_t n = 4;
    ctpl::thread_pool tp(n);
    std::array<std::future<void>, n> returnAdd;
    std::array<std::atomic<int>, n> aArray;

    for (auto _ : state)
    {
        std::fill(aArray.begin(), aArray.end(), 0);
        for(size_t i = 0; i < n; i++)
        {
            returnAdd[i] = tp.push(add_atomic_n, std::ref(aArray[i]), int(state.range(0) / n));
        }
        std::for_each(returnAdd.begin(), returnAdd.end(), [](std::future<void>& f) { f.get(); });
        int result = std::accumulate(aArray.begin(), aArray.end(), 0);
        assert(result == state.range(0));

        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ThreadAtomic4SeparateIntSum)->Range(fromRange, toRange)->UseRealTime()->Threads(4);

static void BM_ThreadAtomic2SeparateCachelineIntSum(benchmark::State& state)
{
    const size_t n = 2;
    ctpl::thread_pool tp(n);
    std::array<std::future<void>, n> returnAdd;
    std::array<std::atomic<int>, n<<4>aArray;

    for (auto _ : state)
    {
        std::fill(aArray.begin(), aArray.end(), 0);
        for(size_t i = 0; i < n; i++)
        {
            returnAdd[i] = tp.push(add_atomic_n, std::ref(aArray[i<<4]), int(state.range(0) / n));
        }
        std::for_each(returnAdd.begin(), returnAdd.end(), [](std::future<void>& f) { f.get(); });
        int result = std::accumulate(aArray.begin(), aArray.end(), 0);
        assert(result == state.range(0));

        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ThreadAtomic2SeparateCachelineIntSum)->Range(fromRange, toRange)->UseRealTime()->Threads(2);

static void BM_ThreadAtomic4SeparateCachelineIntSum(benchmark::State& state)
{
    const size_t n = 4;
    ctpl::thread_pool tp(n);
    std::array<std::future<void>, n> returnAdd;
    std::array<std::atomic<int>, n<<4>aArray;

    for (auto _ : state)
    {
        std::fill(aArray.begin(), aArray.end(), 0);
        for(size_t i = 0; i < n; i++)
        {
            returnAdd[i] = tp.push(add_atomic_n, std::ref(aArray[i<<4]), int(state.range(0) / n));
        }
        std::for_each(returnAdd.begin(), returnAdd.end(), [](std::future<void>& f) { f.get(); });
        int result = std::accumulate(aArray.begin(), aArray.end(), 0);
        assert(result == state.range(0));

        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ThreadAtomic4SeparateCachelineIntSum)->Range(fromRange, toRange)->UseRealTime()->Threads(4);