//
// Created by efarhan on 02/03/2020.
//

#include <random>
#include <vector>
#include <iostream>
#include <benchmark/benchmark.h>
#include "container/custom_allocator.h"

const long fromRange = 8;
const long toRange = 1 << 15;

static void BM_StdVectorAllocate(benchmark::State& state)
{
    size_t size = state.range(0);
    for (auto _ : state)
    {
        std::vector<size_t> v;
        //v.reserve(size);
        for (size_t i = 0; i < size; i++)
        {
            v.push_back(i);
            benchmark::DoNotOptimize(v);
        }
    }
}

BENCHMARK(BM_StdVectorAllocate)->Range(fromRange, toRange);

static void BM_LinearAllocate(benchmark::State& state)
{
    size_t size = state.range(0);
    size_t* values = (size_t*) calloc(size+1, sizeof(size_t));
    size_t maxMemory = 0;
    for (auto _ : state)
    {
        neko::LinearAllocator allocator((size+1) * sizeof(size_t), values);
        size_t* v = (size_t*) allocator.Allocate(sizeof(size_t) * size, alignof(size_t));
        for (size_t i = 0; i < size; i++)
        {
            v[i] = i;
            benchmark::DoNotOptimize(v);
        }
        maxMemory = std::max(maxMemory, allocator.GetUsedMemory());
        allocator.Clear();
    }

    //std::cout << "Linear Allocator memory usage: "<<maxMemory<<"B for size: "<<size<<'\n';
    free(values);
}

BENCHMARK(BM_LinearAllocate)->Range(fromRange, toRange);

static void BM_StackAllocate(benchmark::State& state)
{
    size_t size = state.range(0);
    size_t* values = (size_t*) calloc(size * 2lu, sizeof(size_t));

    size_t maxMemory = 0;
    size_t stackSize = 0;
    for (auto _ : state)
    {
        neko::StackAllocator allocator(size * sizeof(size_t) * 2lu, values);

        size_t* v = (size_t*) allocator.Allocate(sizeof(size_t) * size, alignof(size_t));
        for (size_t i = 0; i < size; i++)
        {
            v[i] = i;
            benchmark::DoNotOptimize(v);
        }
        allocator.Deallocate(v);
        maxMemory = std::max(maxMemory, allocator.GetUsedMemory());
        stackSize = std::max(stackSize, allocator.GetSize());


    }
    //std::cout << "Stack Allocator memory usage: "<<maxMemory<<"B for size: "<<size<<" with stack size: "<<stackSize<<'\n';
    free(values);
}

BENCHMARK(BM_StackAllocate)->Range(fromRange, toRange);

static void BM_PoolAllocate(benchmark::State& state)
{
    size_t size = state.range(0);
    size_t* values = (size_t*) calloc(size, sizeof(size_t));
    size_t maxMemory = 0;
    for (auto _ : state)
    {
        neko::PoolAllocator<size_t> allocator(size * sizeof(size_t), values);
        for (size_t i = 0; i < size; i++)
        {
            size_t* v = (size_t*) allocator.Allocate(sizeof(size_t), alignof(size_t));
            *v = i;
            benchmark::DoNotOptimize(v);
        }
        maxMemory = std::max(maxMemory, allocator.GetUsedMemory());

    }
    //std::cout << "Pool Allocator memory usage: "<<maxMemory<<"B for size: "<<size<<'\n';
    free(values);
}

BENCHMARK(BM_PoolAllocate)->Range(fromRange, toRange);