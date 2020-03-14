/*#include <benchmark/benchmark.h>
#include <mathematics/map.h> // FixedMap = 24 bytes
#include <map> // 48 bytes
#include <unordered_map> // 56 bytes
#include <random_fill.h>
#include <string>

// DISABLE NEKO_ASSERT FOR BENCHMARKING!

namespace neko
{

const size_t FROM = 2;
const size_t TO = 512;

struct DummyStruct // 12 bytes
{
    int a; // 4 bytes
    int b; // 4 bytes // Size of a ptr: 8 bytes
    char c; // 1 byte
    // 3 bytes padding
};

using Key = std::string;
using Value = DummyStruct; // 12 bytes

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

template<size_t SIZE>
DummyStruct const AccessFixedMap(FixedMap<Key, Value, SIZE>& map, const Key key)
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

template<size_t SIZE>
void InsertFixedMap(FixedMap<Key, Value, SIZE>& map, const Key key, const Value value)
{
    map.Append({key, value});
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

/*template<size_t SIZE>
FixedMap<Key, Value, SIZE> InitFixedMap()
{
    return FixedMap<Key, Value, SIZE>();
}*/

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
    const size_t size = s.range(0);
    for (auto _ : s)
    {
        InitStdUnorderedMap(size);
    }
}

/*static void BM_InitFixedMap(benchmark::State& s)
{
    const size_t size = s.range(0);
    for (auto _ : s)
    {
        InitFixedMap<size>();
    }
}*/

static void BM_AccessStdMap(benchmark::State& s)
{
    std::map<Key, Value> map;
    std::vector<Key> keys;
    keys.resize(s.range(0));

    for (auto& key : keys)
    {
        keys.emplace_back(RandomString<FROM,TO>());
        map.insert({
                           keys.back(),
                           DummyStruct{
                                   RandomInt<FROM,TO>(),
                                   RandomInt<FROM,TO>(),
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
    keys.resize(s.range(0));

    for (auto& key : keys)
    {
        keys.emplace_back(RandomString<FROM,TO>());
        map.insert({
                           keys.back(),
                           DummyStruct{
                                   RandomInt<FROM,TO>(),
                                   RandomInt<FROM,TO>(),
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
    const size_t len = s.range(0);
    FixedMap<Key, Value, len> map;
    std::vector<Key> keys;
    keys.resize(s.range(0));

    for (auto& key : keys)
    {
        keys.emplace_back(RandomString<FROM,TO>());
        map.Append(
                keys.back(),
                DummyStruct{
                        RandomInt<FROM,TO>(),
                        RandomInt<FROM,TO>(),
                        RandomChar()});
    }

    for (auto _ : s)
    {
        for (auto key : keys)
        {
            AccessFixedMap<len>(map, key);
        }
    }
}

static void BM_InsertStdMap(benchmark::State& s)
{
    std::map<Key, Value> map;
    std::vector<std::pair<Key, Value>> pairs; // pair = 8 + 12 => 20 bytes // Q: alignof pair? 12? if so => sizeof pair = 24 bytes
    pairs.resize(s.range(0));

    for (auto& pair : pairs)
    {
        pairs.emplace_back({
                                   RandomString<FROM,TO>(),
                                   DummyStruct{
                                           RandomInt<FROM,TO>(),
                                           RandomInt<FROM,TO>(),
                                           RandomChar()}});
    }

    for (auto _ : s)
    {
        for (auto pair : pairs)
        {
            InsertStdMap(map, pair.first, pair.second);
        }
    }
}

static void BM_InsertStdUnorderedMap(benchmark::State& s)
{
    std::unordered_map<Key, Value> map;
    std::vector<std::pair<Key, Value>> pairs;
    pairs.resize(s.range(0));
    map.reserve(s.range(0));

    for (auto& pair : pairs)
    {
        pairs.emplace_back({
                                   RandomString<FROM,TO>(),
                                   DummyStruct{
                                           RandomInt<FROM,TO>(),
                                           RandomInt<FROM,TO>(),
                                           RandomChar()}});
    }

    for (auto _ : s)
    {
        for (auto pair : pairs)
        {
            InsertStdUnorderedMap(map, pair.first, pair.second);
        }
    }
}

static void BM_InsertFixedMap(benchmark::State& s)
{
    const size_t len = s.range(0);
    FixedMap<Key, Value, len> map;
    std::vector<std::pair<Key, Value>> pairs;
    pairs.resize(len);

    for (auto& pair : pairs)
    {
        pairs.emplace_back({
                                   RandomString<FROM,TO>(),
                                   DummyStruct{
                                           RandomInt<FROM,TO>(),
                                           RandomInt<FROM,TO>(),
                                           RandomChar()}});
    }

    for (auto _ : s)
    {
        for (auto pair : pairs)
        {
            InsertFixedMap<len>(map, pair.first, pair.second);
        }
    }
}

BENCHMARK(BM_InitStdMap)->Range(FROM, TO);
BENCHMARK(BM_InitStdUnorderedMap)->Range(FROM, TO);
BENCHMARK(BM_InitFixedMap)->Range(FROM, TO);
BENCHMARK(BM_AccessStdMap)->Range(FROM, TO);
BENCHMARK(BM_AccessStdUnorderedMap)->Range(FROM, TO);
BENCHMARK(BM_AccessFixedMap)->Range(FROM, TO);
BENCHMARK(BM_InsertStdMap)->Range(FROM, TO);
BENCHMARK(BM_InsertStdUnorderedMap)->Range(FROM, TO);
BENCHMARK(BM_InsertFixedMap)->Range(FROM, TO);

BENCHMARK_MAIN();

}// !neko
*/