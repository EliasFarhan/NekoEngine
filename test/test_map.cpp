#include <gtest/gtest.h>
#include <mathematics/map.h>
#include <vector>
#include <iostream>

namespace neko
{

TEST(Engine, Map)
{
    const size_t sizeInBytes = 512;

    neko::Map<std::uint8_t, double> map((neko::Allocator*) nullptr, sizeInBytes);

    {// Set / retrieve.
        const uint8_t key[] = {1u, 2u, 3u, 4u, 5u};
        const double value[] = {1.0, 2.0, 3.0, 4.0, 5.0};
        for (uint8_t i = 0; i < 5u; i++)
        {
            const bool result = map.Add(key[i], value[i]);
            EXPECT_TRUE(result);
        }
        for (uint8_t i = 0; i < 5; i++)
        {
            EXPECT_EQ(value[i], map.FindValue(key[i]));
        }
    }

    {// Swap.
        const double value = map.FindValue(1u);
        const double expectedSum = 1.0 + 2.0 + 3.0 + 4.0 + 5.0;
        double sum = 0.0;
        for (uint8_t i = 1u; i < 5u; i++)
        {
            const bool result = map.Swap(i, i + 1u);
            EXPECT_TRUE(result);
        }
        for (uint8_t i = 1u; i < 6u; i++)
        {
            sum += map.FindValue(i);
        }
        EXPECT_EQ(value, map.FindValue(5u));
        EXPECT_EQ(expectedSum, sum);
    }

    {// Clear, remove and add.
        map.Clear();

        const uint8_t key[] = {1u, 2u, 3u, 4u, 5u};
        const double value[] = {1.0, 2.0, 3.0, 4.0, 5.0};
        for (uint8_t i = 0; i < 5u; i++)
        {
            map.Add(key[i], value[i]);
        }

        map.Remove(2u);
        map.Remove(4u);

        double expectedSum = 1.0 + 3.0 + 5.0;
        double sum = 0.0;
        for (uint8_t i = 1u; i < 6u; i++)
        {
            sum += map.FindValue(i);
        }
        EXPECT_EQ(expectedSum, sum);

        map.Add(2u, 2.0);
        map.Add(4u, 4.0);

        expectedSum = 1.0 + 2.0 + 3.0 + 4.0 + 5.0;
        sum = 0.0;
        for (uint8_t i = 1u; i < 6u; i++)
        {
            sum += map.FindValue(i);
        }
        EXPECT_EQ(expectedSum, sum);
    }
}
/*
TEST(Engine, MinimalMap)
{
    std::cout << "Size of Key: " << sizeof(unsigned int) << '\n';
    std::cout << "Align of Key: " << alignof(unsigned int) << '\n';
    std::cout << "Size of Value: " << sizeof(float) << '\n';
    std::cout << "Align of Value: " << alignof(float) << '\n';
    std::cout << "Size of Hash: " << sizeof(xxh::hash_t<64>) << '\n';
    std::cout << "Align of Hash: " << alignof(xxh::hash_t<64>) << '\n';
    std::cout << "Size of Pair: " << sizeof(std::pair<xxh::hash_t<64>, float>) << '\n';
    std::cout << "Align of Pair: " << alignof(std::pair<xxh::hash_t<64>, float>) << '\n';

    const size_t sizeInNumberOfCacheLines = 1;
    const size_t cacheLineSize = 64;
    MinimalMap<unsigned int, float> map(sizeInNumberOfCacheLines);

    std::cout << "Size of map: " << sizeof(map) << '\n';
    std::cout << "Align of map: " << alignof(map) << '\n';

    std::vector<unsigned int> keys;
    std::vector<float> values;

    const size_t len = cacheLineSize / sizeof(std::pair<xxh::hash_t<64>, float>);

    for (unsigned int i = 0; i < len; i++)
    {
        keys.push_back(i);
        values.push_back(rand());
    }

    for (unsigned int i = 0; i < len; i++)
    {
        EXPECT_TRUE(map.Add(keys[i], values[i]));
    }

    std::vector<float> retrievedValues;
    for (unsigned int i = 0; i < len; i++)
    {
        retrievedValues.push_back(map.Retrieve(keys[i]));
        EXPECT_EQ(retrievedValues[i], values[i]);
    }

    for (unsigned int i = 0; i < len; i++)
    {
        EXPECT_TRUE(map.Remove(keys[i]));
    }

    for (unsigned int i = 0; i < len; i++)
    {
        EXPECT_EQ(0.0f, map.Retrieve(i));
    }
}*/

TEST(Engine, PoolAllocatorMap)
{
    const uint8_t sizeInNumberOfCacheLines = 1;
    const size_t sizeOfCacheLine = 64;
    const size_t sizeOfPair = sizeof(std::pair<xxh::hash_t<64>, float>);

    PoolAllocatorMap<unsigned int, float> map(sizeInNumberOfCacheLines);

    std::vector<float> values;
    for (size_t i = 0; i < (sizeOfCacheLine * sizeInNumberOfCacheLines) / sizeOfPair; i++)
    {
        values.push_back(rand());
    }

    // Addition.
    for (unsigned int i = 0; i < (sizeOfCacheLine * sizeInNumberOfCacheLines) / sizeOfPair; i++)
    {
        map.Add(i, values[i]);
    }

    float sum = 0;
    for (auto* pair : map.pairs_){
        sum += pair->second;
    }
    float expected = 0;
    for (auto value : values){
        expected += value;
    }
    EXPECT_EQ(sum, expected);

    // Retrieval.
}

}// !neko