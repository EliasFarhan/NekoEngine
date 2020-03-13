#include <gtest/gtest.h>
#include <mathematics/map.h>
#include <vector>
#include <algorithm>

namespace neko
{

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
    for (size_t i = 0; i < MAP_SIZE; ++i)
    {
        sum -= values[i];
    }
    EXPECT_EQ(expectedSum, sum);
    sum = 0;

    // Iterators testing.
    std::for_each(map.begin(), map.end(), [](std::pair<xxh::hash_t<64>, float>& p) {
        p.first = 0;
        p.second = 0;
    });
    std::for_each(map.begin(), map.end(), [&sum](std::pair<xxh::hash_t<64>, float>& p) { sum += p.second; });
    EXPECT_EQ(0, sum);
}

}// !neko