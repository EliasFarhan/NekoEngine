#include <benchmark/benchmark.h>
#include <utilities/map.h> // FixedMap = 24 bytes
#include <map> // 48 bytes
#include <unordered_map> // 56 bytes
#include <random_fill.h>
#include <string>

// DISABLE NEKO_ASSERT FOR BENCHMARKING!

namespace neko
{

// ----------------------------------------------------------------
// Compile time definitions.
// ----------------------------------------------------------------

const size_t SIZE = 512;
using Key = unsigned long long;
using Value = unsigned long long;
using Pair = std::pair<Key, Value>;
using StdMapType = std::map<Key, Value>;
using StdUnorderedMapType = std::unordered_map<Key, Value>;
using FixedMapType = FixedMap<Key, Value, SIZE>;

// ----------------------------------------------------------------
// Utility functions.
// ----------------------------------------------------------------

Key GetNextKey(){
    static Key k = 0;
    return ++k;
}

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
    return FixedMapType();
}

// ----------------------------------------------------------------
// Init benches.
// ----------------------------------------------------------------

void BM_StdMap_Init(benchmark::State& s)
{
    for (auto _ : s)
    {
        benchmark::DoNotOptimize(StdMap_Init());
    }
}

void BM_StdUnorderedMap_Init(benchmark::State& s)
{
    for (auto _ : s)
    {
        benchmark::DoNotOptimize(StdUnorderedMap_Init());
    }
}

void BM_FixedMap_Init(benchmark::State& s)
{
    for (auto _ : s)
    {
        benchmark::DoNotOptimize(FixedMap_Init());
    }
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
        map.Append(GetNextKey(), GetNextKey());
    }
}

// ----------------------------------------------------------------
// Fill out benches.
// ----------------------------------------------------------------

void BM_StdMap_InitAndFillOut(benchmark::State& s)
{
    for (auto _ : s)
    {
        StdMapType map = StdMap_Init();
        StdMap_FillOut(map);
    }
}

void BM_StdUnorderedMap_InitAndFillOut(benchmark::State& s)
{
    for (auto _ : s)
    {
        StdUnorderedMapType map = StdUnorderedMap_Init();
        StdUnorderedMap_FillOut(map);
    }
}

void BM_FixedMap_InitAndFillOut(benchmark::State& s)
{
    for (auto _ : s)
    {
        FixedMapType map = FixedMap_Init();
        FixedMap_FillOut(map);
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
    std::for_each(map.begin(), map.end(), [](std::pair<xxh::hash_t<64>, Value>& p) {
        p.first = 0;
        p.second = 0;
    });
}

// ----------------------------------------------------------------
// Clear benches.
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
        map.Append(key, GetNextKey());
    }

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

BENCHMARK(BM_StdMap_Init);
BENCHMARK(BM_StdUnorderedMap_Init);
BENCHMARK(BM_FixedMap_Init);
BENCHMARK(BM_StdMap_InitAndFillOut);
BENCHMARK(BM_StdUnorderedMap_InitAndFillOut);
BENCHMARK(BM_FixedMap_InitAndFillOut);
BENCHMARK(BM_StdMap_InitFillOutAndClear);
BENCHMARK(BM_StdUnorderedMap_InitFillOutAndClear);
BENCHMARK(BM_FixedMap_InitFillOutAndClear);
BENCHMARK(BM_StdMap_Access);
BENCHMARK(BM_StdUnorderedMap_Access);
BENCHMARK(BM_FixedMap_Access);

BENCHMARK_MAIN();

}// !neko
