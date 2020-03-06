#include <gtest/gtest.h>
#include <mathematics/map.h>
#include <engine/custom_allocator.h>
#include <xxhash.hpp>

namespace neko{

TEST(Engine, Map)
{
    const size_t sizeInBytes = 512;

    void* mem = malloc(sizeInBytes);
    const auto allocator = PoolAllocator<std::pair<xxh::hash_t<64>, double>>(sizeInBytes, mem);

    Map<std::uint8_t, double> map((Allocator*) &allocator, sizeInBytes);

    {// Set / retrieve.
        const uint8_t key[] = {1u, 2u, 3u, 4u, 5u};
        const double value[] = {1.0, 2.0, 3.0, 4.0, 5.0};
        for (uint8_t i = 0; i < 5u ; i++)
        {
            map.Add(key[i], value[i]);
        }
        for (uint8_t i = 0; i < 5 ; i++)
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
            map.Swap(i, i+1u);
        }
        for (uint8_t i = 1u; i < 6u; i++)
        {
            sum += map.FindValue(i);
        }
        EXPECT_EQ(value, map.FindValue(5u));
        EXPECT_EQ(expectedSum, sum);
    }

    {// Remove and add.
        double values[5];
        for (uint8_t i = 0; i < 5u ; ++i)
        {
            values[i] = map.FindValue(i);
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

    {// Clear.
        void* startOfVector = &map.FindValue(1u);

        double sum = 0.0;
        for (size_t i = 0; i < sizeInBytes / sizeof(double) ; i++)
        {
            sum += *(static_cast<double*>(startOfVector));
            // TODO@Oleg: finish this.
        }
        EXPECT_EQ(sum, 0.0);
    }

    free(mem);
}

}