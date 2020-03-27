#include <benchmark/benchmark.h>
#include <custom_container/map.h> // FixedMap = 24 bytes
#include <map> // 48 bytes
#include <unordered_map> // 56 bytes
#include <random_fill.h>
#include <string>

namespace neko
{

#define HASH_SIZE 32

// ----------------------------------------------------------------
// Compile time definitions.
// ----------------------------------------------------------------

const size_t SIZE = 128; // max nbr of distinct chars
using Key = unsigned char;
using Value = unsigned char;
using Pair = std::pair<Key, Value>;
using InternalPair = std::pair<xxh::hash_t<HASH_SIZE>, Value>;
using StdMapType = std::map<Key, Value>;
using StdUnorderedMapType = std::unordered_map<Key, Value>;
using FixedMapType = FixedMap<Key, Value, SIZE>;
const size_t ALLOCATOR_HEADER_SIZE = 0;
const size_t ALIGNMENT_PADDING = 1;
const size_t INTERNAL_PAIR_SIZE = sizeof(InternalPair);
const size_t HEAP_SIZE = ALLOCATOR_HEADER_SIZE + (SIZE * INTERNAL_PAIR_SIZE) + ALIGNMENT_PADDING;
using AllocatorType = LinearAllocator;

// ----------------------------------------------------------------
// Utility functions.
// ----------------------------------------------------------------

Key GetNextKey(){
    static Key k = 0;
    return ++k;
}

// ----------------------------------------------------------------
// Benchmark wide allocator.
// ----------------------------------------------------------------

AllocatorType allocator(HEAP_SIZE, malloc(HEAP_SIZE));

// ----------------------------------------------------------------
// Init.
// ----------------------------------------------------------------

inline StdMapType StdMap_Init()
{
    return StdMapType();
}

inline StdUnorderedMapType StdUnorderedMap_Init()
{
    return StdUnorderedMapType(SIZE);
}

inline FixedMapType FixedMap_Init()
{
    return FixedMapType(allocator);
}

// ----------------------------------------------------------------
// Fill out.
// ----------------------------------------------------------------
inline void StdMap_FillOut(StdMapType& map)
{
    for (size_t i = 0; i < SIZE; i++)
    {
        map.insert({GetNextKey(), GetNextKey()});
    }
}

inline void StdUnorderedMap_FillOut(StdUnorderedMapType& map)
{
    for (size_t i = 0; i < SIZE; i++)
    {
        map.insert({GetNextKey(), GetNextKey()});
    }
}

inline void FixedMap_FillOut(FixedMapType& map)
{
    for (size_t i = 0; i < SIZE; i++)
    {
        map.Insert({GetNextKey(), GetNextKey()});
    }
}

// ----------------------------------------------------------------
// Clear.
// ----------------------------------------------------------------

inline void StdMap_Clear(StdMapType& map)
{
    return map.clear();
}

inline void StdUnorderedMap_Clear(StdUnorderedMapType& map)
{
    return map.clear();
}

inline void FixedMap_Clear(FixedMapType& map)
{
    return map.Clear();
}

// ----------------------------------------------------------------
// InitFillOutAndClear benches.
// ----------------------------------------------------------------

void BM_StdMap_InitFillOutAndClear(benchmark::State& s)
{
    for (auto _ : s)
    {
        StdMapType map = StdMap_Init();
        StdMap_FillOut(map);
        StdMap_Clear(map);
    }
}

void BM_StdUnorderedMap_InitFillOutAndClear(benchmark::State& s)
{
    for (auto _ : s)
    {
        StdUnorderedMapType map = StdUnorderedMap_Init();
        StdUnorderedMap_FillOut(map);
        StdUnorderedMap_Clear(map);
    }
}

void BM_FixedMap_InitFillOutAndClear(benchmark::State& s)
{
    for (auto _ : s)
    {
        FixedMapType map = FixedMap_Init();
        FixedMap_FillOut(map);
        FixedMap_Clear(map);
    }
}

// ----------------------------------------------------------------
// Access.
// ----------------------------------------------------------------

inline Value StdMap_Access(StdMapType& map, const Key key)
{
    return map[key];
}

inline Value StdUnorderedMap_Access(StdUnorderedMapType& map, const Key key)
{
    return map[key];
}

inline Value FixedMap_Access(FixedMapType& map, const Key key)
{
    return map[key];
}

// ----------------------------------------------------------------
// Access benches.
// ----------------------------------------------------------------

void BM_StdMap_Access(benchmark::State& s)
{
    StdMapType map = StdMap_Init();
    std::vector<Key> keys(SIZE);
    for (auto& key : keys)
    {
        key = GetNextKey();
        map.insert({key, GetNextKey()});
    }

    for (auto _ : s)
    {
        for (auto key : keys)
        {
            benchmark::DoNotOptimize(StdMap_Access(map, key));
        }
    }
}

void BM_StdUnorderedMap_Access(benchmark::State& s)
{
    StdUnorderedMapType map = StdUnorderedMap_Init();
    std::vector<Key> keys(SIZE);
    for (auto& key : keys)
    {
        key = GetNextKey();
        map.insert({key, GetNextKey()});
    }

    for (auto _ : s)
    {
        for (auto key : keys)
        {
            benchmark::DoNotOptimize(StdUnorderedMap_Access(map, key));
        }
    }
}

void BM_FixedMap_Access(benchmark::State& s)
{
    FixedMapType map = FixedMap_Init();
    std::vector<Key> keys(SIZE);
    for (auto& key : keys)
    {
        key = GetNextKey();
        map.Insert({key, GetNextKey()});
    }
    map.Rearrange();

    for (auto _ : s)
    {
        for (auto key : keys)
        {
            benchmark::DoNotOptimize(FixedMap_Access(map, key));
        }
    }
}

// ----------------------------------------------------------------
// Register benches and run them.
// ----------------------------------------------------------------

BENCHMARK(BM_StdMap_InitFillOutAndClear);
BENCHMARK(BM_StdUnorderedMap_InitFillOutAndClear);
BENCHMARK(BM_FixedMap_InitFillOutAndClear);
BENCHMARK(BM_StdMap_Access);
BENCHMARK(BM_StdUnorderedMap_Access);
BENCHMARK(BM_FixedMap_Access);

BENCHMARK_MAIN();

#undef HASH_SIZE

}// !neko
