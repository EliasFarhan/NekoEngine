// TODO: Find a way to profile maps of a defined size. Currnetly profiling maps of SIZE * numOfGoogleIterations...

/*#include <benchmark/benchmark.h>
#include <mathematics/map.h> // FixedMap = 24 bytes
#include <map> // 48 bytes
#include <unordered_map> // 56 bytes
#include <random_fill.h>
#include <string>

// DISABLE NEKO_ASSERT FOR BENCHMARKING!

namespace neko
{

const size_t FIXED_MAP_SIZE = 512;

struct DummyStruct // 12 bytes
{
    int a; // 4 bytes
    int b; // 4 bytes // Size of a ptr: 8 bytes
    char c; // 1 byte
    // 3 bytes padding
};

using Key = unsigned long int;
using Value = DummyStruct; // 12 bytes

static Key GetNextChar()
{
    static Key c = 0;
    return ++c;
}

static void ClearStdMap(std::map<Key, Value>& map)
{
    map.clear();
}

static void ClearStdUnorderedMap(std::unordered_map<Key, Value>& map)
{
    map.clear();
}

static void ClearFixedMap(FixedMap<Key, Value, FIXED_MAP_SIZE>& map)
{
    std::for_each(map.begin(), map.end(), [](std::pair<Key, Value>& p) {
        p.first = 0;
        p.second = {0, 0, 0};
    });
}

// ----------------------------------------------------------------
// Access
// ----------------------------------------------------------------

DummyStruct const AccessStdMap(std::map<Key, Value>& map, const Key key)
{
    return map[key];
}

DummyStruct const AccessStdUnorderedMap(std::unordered_map<Key, Value>& map, const Key key)
{
    return map[key];
}

DummyStruct const AccessFixedMap(FixedMap<Key, Value, FIXED_MAP_SIZE>& map, const Key key)
{
    return map[key];
}

// ----------------------------------------------------------------
// Insert
// ----------------------------------------------------------------

void InsertStdMap(std::map<Key, Value>& map, const Key key, const Value value)
{
    map.insert({key, value});
}

void InsertStdUnorderedMap(std::unordered_map<Key, Value>& map, const Key key, const Value value)
{
    map.insert({key, value});
}

void InsertFixedMap(FixedMap<Key, Value, FIXED_MAP_SIZE>& map, const Key key, const Value value)
{
    map.Append(key, value);
}

// ----------------------------------------------------------------
// Init
// ----------------------------------------------------------------

std::map<Key, Value> InitStdMap()
{
    return std::map<Key, Value>();
}

std::unordered_map<Key, Value> InitStdUnorderedMap(const size_t size)
{
    auto returnValue = std::unordered_map<Key, Value>();
    returnValue.reserve(size);
    return returnValue;
}

FixedMap<Key, Value, FIXED_MAP_SIZE> InitFixedMap()
{
    return FixedMap<Key, Value, FIXED_MAP_SIZE>();
}

// ----------------------------------------------------------------
// Benches
// ----------------------------------------------------------------

static void BM_InitStdMap(benchmark::State& s)
{
    for (auto _ : s)
    {
        InitStdMap();
    }
}

static void BM_InitStdUnorderedMap(benchmark::State& s)
{
    for (auto _ : s)
    {
        InitStdUnorderedMap(FIXED_MAP_SIZE);
    }
}

static void BM_InitFixedMap(benchmark::State& s)
{
    for (auto _ : s)
    {
        InitFixedMap();
    }
}

static void BM_AccessStdMap(benchmark::State& s)
{
    std::map<Key, Value> map;
    std::vector<Key> keys;
    keys.resize(FIXED_MAP_SIZE);

    for (auto& key : keys)
    {
        keys.emplace_back(GetNextChar());
        map.insert({
                           keys.back(),
                           DummyStruct{
                                   RandomInt<2, 512>(),
                                   RandomInt<2, 512>(),
                                   RandomChar()}});
    }

    for (auto _ : s)
    {
        for (auto key : keys)
        {
            AccessStdMap(map, key);
        }
    }
}

static void BM_AccessStdUnorderedMap(benchmark::State& s)
{
    std::unordered_map<Key, Value> map;
    std::vector<Key> keys;
    keys.resize(FIXED_MAP_SIZE);

    for (auto& key : keys)
    {
        keys.emplace_back(GetNextChar());
        map.insert({
                           keys.back(),
                           DummyStruct{
                                   RandomInt<2, 512>(),
                                   RandomInt<2, 512>(),
                                   RandomChar()}});
    }

    for (auto _ : s)
    {
        for (auto key : keys)
        {
            AccessStdUnorderedMap(map, key);
        }
    }
}

static void BM_AccessFixedMap(benchmark::State& s)
{
    const size_t len = FIXED_MAP_SIZE;
    FixedMap<Key, Value, len> map;
    std::vector<Key> keys;
    keys.resize(len);

    for (auto& key : keys)
    {
        keys.emplace_back(GetNextChar());
        map.Append(
                keys.back(),
                DummyStruct{
                        RandomInt<2, 512>(),
                        RandomInt<2, 512>(),
                        RandomChar()});

    }

    for (auto _ : s)
    {
        for (auto key : keys)
        {
            AccessFixedMap(map, key);
        }
    }
}

static void BM_InsertStdMap(benchmark::State& s)
{
    std::map<Key, Value> map;
    std::vector<std::pair<Key, Value>> pairs; // pair = 8 + 12 => 20 bytes // Q: alignof pair? 12? if so => sizeof pair = 24 bytes
    pairs.resize(FIXED_MAP_SIZE);

    for (auto& pair : pairs)
    {
        pair.first = GetNextChar();
        pair.second = DummyStruct{RandomInt<2, 512>(), RandomInt<2, 512>(), RandomChar()};
    }

    for (auto _ : s)
    {
        for (auto pair : pairs)
        {
            InsertStdMap(map, pair.first, pair.second);
        }
        ClearStdMap(map);
    }
}

static void BM_InsertStdUnorderedMap(benchmark::State& s)
{
    std::unordered_map<Key, Value> map;
    std::vector<std::pair<Key, Value>> pairs;
    pairs.resize(FIXED_MAP_SIZE);
    map.reserve(FIXED_MAP_SIZE);

    for (auto& pair : pairs)
    {
        pair.first = GetNextChar();
        pair.second = DummyStruct{RandomInt<2, 512>(), RandomInt<2, 512>(), RandomChar()};
    }

    for (auto _ : s)
    {
        for (auto pair : pairs)
        {
            InsertStdUnorderedMap(map, pair.first, pair.second);
        }
        ClearStdUnorderedMap(map);
    }
}

static void BM_InsertFixedMap(benchmark::State& s)
{
    const size_t len = FIXED_MAP_SIZE;
    FixedMap<Key, Value, len> map;
    std::vector<std::pair<Key, Value>> pairs;
    pairs.resize(len);

    for (auto& pair : pairs)
    {
        pair.first = GetNextChar();
        pair.second = DummyStruct{RandomInt<2, 512>(), RandomInt<2, 512>(), RandomChar()};
    }

    for (auto _ : s)
    {
        for (auto pair : pairs)
        {
            InsertFixedMap(map, pair.first, pair.second);
        }
        ClearFixedMap(map);
    }
}

BENCHMARK(BM_InitStdMap);
BENCHMARK(BM_InitStdUnorderedMap);
BENCHMARK(BM_InitFixedMap);
BENCHMARK(BM_InsertStdMap);
BENCHMARK(BM_InsertStdUnorderedMap);
BENCHMARK(BM_InsertFixedMap);
BENCHMARK(BM_AccessStdMap);
BENCHMARK(BM_AccessStdUnorderedMap);
BENCHMARK(BM_AccessFixedMap);

BENCHMARK_MAIN();

}// !neko
*/