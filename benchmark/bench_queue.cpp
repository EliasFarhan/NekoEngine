#include <utilities/custom_queue.h>
#include <benchmark/benchmark.h>

const unsigned long fromRange = 8;
const unsigned long toRange = 1 << 10;

static void BM_Queue(benchmark::State& state)
{
    const auto length = state.range(0);
    neko::DynamicQueue<int> queue;
   for (int i = 0; i < length; i++) 
    {
       
       queue.PushBack(rand()% 32768);
       
   }
 
    for (auto _ : state)
    {
        for (size_t i = 0; i < queue.Size(); i++)
        {
            benchmark::DoNotOptimize(queue);
        }
    	       queue.PopFront();

    }
  
}
BENCHMARK(BM_Queue)->Range(fromRange, toRange);

static void BM_QueueBasic(benchmark::State& state)
{
    const auto length = state.range(0);
    neko::DynamicQueueBasic<int> queue;
    for (int i = 0; i < length; i++)
    {

        queue.PushBack(rand() % 32768);

    }

    for (auto _ : state)
    {
        for (size_t i = 0; i < queue.Size(); i++)
        {
            benchmark::DoNotOptimize(queue);
        }
        queue.PopFront();

    }
}
BENCHMARK(BM_QueueBasic)->Range(fromRange, toRange);