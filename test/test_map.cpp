#include <gtest/gtest.h>
#include <mathematics/map.h>
#include <vector>
#include <algorithm>

namespace neko
{

/*TEST(Engine, AllocatorUsingFixedMap){
    const size_t SIZE = 4;
    using Hash = xxh::hash_t<64>;
    using Key = unsigned int;
    using Value = unsigned int;
    using Pair = std::pair<Key, Value>;
    using HashPair = std::pair<Hash, Value>;
    using Map = FixedMap<Key,Value,SIZE>;
    const size_t HASH_PAIR_SIZE = sizeof(HashPair);

    std::array<Key, SIZE> keys{1,2,3,4};
    std::array<Value, SIZE> values{1,2,3,4};

    const size_t TOTAL_NEEDED_SIZE = (HASH_PAIR_SIZE * SIZE) + 1;
    void* mem = malloc(TOTAL_NEEDED_SIZE);
    LinearAllocator allocator(TOTAL_NEEDED_SIZE, mem);

    Map map(allocator);
    for (size_t i = 0; i < SIZE; i++)
    {
        map.Append({keys[i], values[i]});
    }

    allocator.Clear();
    free(mem);
}*/

TEST(Engine, FixedMap)
{
    const size_t MAP_SIZE = 4;

    FixedMap<unsigned int, float, MAP_SIZE> map = FixedMap<unsigned int, float, MAP_SIZE>();
    float expectedSum = 0;
    float sum = 0;

    std::array<unsigned int, MAP_SIZE> keys{0};
    std::array<float, MAP_SIZE> values{0};
    for (size_t i = 1; i < MAP_SIZE + 1; i++)
    {
        keys[i - 1] = i;
        values[i - 1] = i;
        expectedSum += i;
    }
    for (size_t i = 1; i < MAP_SIZE + 1; i++)
    {
        map.Append(keys[i - 1], values[i - 1]);
    }

    // [] testing.
    for (size_t i = 1; i < MAP_SIZE + 1; i++)
    {
        sum += map[i];
    }
    EXPECT_EQ(expectedSum, sum);
    sum = 0;

    std::for_each(values.begin(), values.end(), [](float& f) { f = -f; }); // Negate all values.
    for (size_t i = 1; i < MAP_SIZE + 1; i++)
    {
        map[i] = values[i - 1];
    }
    for (size_t i = 1; i < MAP_SIZE + 1; ++i)
    {
        sum -= map[i];
    }
    EXPECT_EQ(expectedSum, sum);
    sum = 0;

    // Iterators testing.
    FixedMap<unsigned int, float, MAP_SIZE> map1 = map;
    std::for_each(map.begin(), map.end(), [](std::pair<xxh::hash_t<64>, float>& p) {
        p.first = 0;
        p.second = 0;
    });
    std::for_each(map.begin(), map.end(), [&sum](std::pair<xxh::hash_t<64>, float>& p) { sum += p.second; });
    EXPECT_EQ(0, sum);

    // Assignment testing.
    map = map1;
    std::for_each(map.begin(), map.end(), [&sum](std::pair<xxh::hash_t<64>, float>& p) {
        sum -= p.second;
    });
    EXPECT_EQ(expectedSum, sum);
    sum = 0;

    // Remove and clear testing.
    for (auto& key : keys)
    {
        map.Remove(key);
    }
    std::for_each(map.begin(), map.end(), [&sum](std::pair<xxh::hash_t<64>, float>& p) {
        sum += p.second;
    });
    EXPECT_EQ(0, sum);

    map = map1;
    map.Clear();
    std::for_each(map.begin(), map.end(), [&sum](std::pair<xxh::hash_t<64>, float>& p) {
        sum += p.second;
    });
    EXPECT_EQ(0, sum);
}

}// !neko