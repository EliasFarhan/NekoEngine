#include <utilities/custom_queue.h>
#include <benchmark/benchmark.h>
#include <queue>

const unsigned long fromRange = 8;
const unsigned long toRange = 1 << 15;

static void BM_Queue(benchmark::State& state)
{
	for (auto _ : state)
	{
		const auto length = state.range(0);
		neko::DynamicQueue<int> queue;
		for (int i = 0; i < length; i++)
		{
			queue.PushBack(rand() % 32768);
			
		}
		for (int i = 0; i < length; i++)
		{
			queue.PopFront();
		}

		
	}
}

BENCHMARK(BM_Queue)->Range(fromRange, toRange);

static void BM_QueueBasic(benchmark::State& state)
{
	for (auto _ : state)
	{
		const auto length = state.range(0);
		neko::DynamicQueueBasic<int> queue;
		for (int i = 0; i < length; i++)
		{
			queue.PushBack(rand() % 32768);
		}
		
		for (int i = 0; i < length; i++)
		{
			queue.PopFront();
		}		
		
	}
}

BENCHMARK(BM_QueueBasic)->Range(fromRange, toRange);
static void BM_stdQueue(benchmark::State& state)
{
	for (auto _ : state)
	{
		const auto length = state.range(0);
		std::queue<int> queue;
		for (int i = 0; i < length; i++)
		{
			queue.push(rand() % 32768);
		}

		for (int i = 0; i < length; i++)
		{
			queue.pop();
		}

	}
}

BENCHMARK(BM_stdQueue)->Range(fromRange, toRange);