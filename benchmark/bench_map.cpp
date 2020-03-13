#include <benchmark/benchmark.h>
#include <mathematics/map.h>
#include <map>
#include <random_fill.h>

const size_t from = 2;
const size_t to = 512;

// Retrieval.

const double AccessStdMap(std::map<int, double>& map, const int key)
{
    return map[key];
}

const double AccessNekoMap(neko::FixedMap<int, double>& map, const int key)
{
    return map.FindValue(key);
}

static void BM_StdMap_Retrieval(benchmark::State& s)
{
    const size_t len = s.range(0);

    std::map<int, double> stdMap;

    for (int i = 0; i < len; ++i)
    {
        const int key = RandomFloat();
        const double value = RandomFloat();
        stdMap.insert(std::pair<int, double>(key,value));
    }

    for (auto _ : s)
    {
        for (int i = 0; i < len; ++i)
        {
            benchmark::DoNotOptimize(AccessStdMap(stdMap, i));
        }
    }
}

static void BM_NekoMap_Retrieval(benchmark::State& s)
{
    const size_t len = s.range(0);

    const size_t sizeInBytes = (sizeof(int) + sizeof(double)) * len;
    neko::FixedMap<int, double> nekoMap(nullptr, sizeInBytes);

    for (int i = 0; i < len; ++i)
    {
        const int key = RandomFloat();
        const double value = RandomFloat();
        nekoMap.Add(key,value);
    }

    for (auto _ : s)
    {
        for (int i = 0; i < len; ++i)
        {
            benchmark::DoNotOptimize(AccessNekoMap(nekoMap, i));
        }
    }
}

BENCHMARK(BM_StdMap_Retrieval)->Range(from, to);
BENCHMARK(BM_NekoMap_Retrieval)->Range(from, to);

// Insertion.

void InsertStdMap(std::map<int, double>& map,const int key,const double value)
{
    map.insert({key,value});
}

void InsertNekoMap(neko::FixedMap<int, double>& map, const int key, const double value)
{
    map.Add(key,value);
}

static void BM_StdMap_Insertion(benchmark::State& s)
{
    const size_t len = s.range(0);
    int keys[len];
    double values[len];
    for (size_t i = 0; i < len; i++)
    {
        keys[i] = RandomFloat();
        values[i] = RandomFloat();
    }

    std::map<int, double> stdMap;

    for (auto _ : s)
    {
        for (int i = 0; i < len; ++i)
        {
            InsertStdMap(stdMap, keys[i], values[i]);
        }
    }
}

static void BM_NekoMap_Insertion(benchmark::State& s)
{
    const size_t len = s.range(0);
    int keys[len];
    double values[len];
    for (size_t i = 0; i < len; i++)
    {
        keys[i] = RandomFloat();
        values[i] = RandomFloat();
    }

    neko::FixedMap<int, double> nekoMap(nullptr, (sizeof(int) + sizeof(double)) * len);

    for (auto _ : s)
    {
        for (int i = 0; i < len; ++i)
        {
            InsertNekoMap(nekoMap, keys[i], values[i]);
        }
    }
}

BENCHMARK(BM_StdMap_Insertion)->Range(from, to);
BENCHMARK(BM_NekoMap_Insertion)->Range(from, to);

BENCHMARK_MAIN();