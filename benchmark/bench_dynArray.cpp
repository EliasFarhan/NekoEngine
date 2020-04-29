#include <benchmark/benchmark.h>
#include <random>
#include "engine/array.h"
#include <mathematics/vector.h>

const unsigned long fromRange = 8;
const unsigned long toRange = 1 << 16;

static void BM_Push(benchmark::State& state){

	const auto length = state.range(0);
	const size_t ALLOCATOR_HEADER_SIZE = 16;
	const size_t SIZE = 4194304;
	void* start = malloc(SIZE + ALLOCATOR_HEADER_SIZE);

	neko::FreeListAllocator allocator(SIZE + ALLOCATOR_HEADER_SIZE, start);
	neko::DynArray<int> dyn(allocator);

	for (int i = 0; i < length; i++){
		dyn.Push(rand()%32768);
	}
}
BENCHMARK(BM_Push)->Range(fromRange, toRange);