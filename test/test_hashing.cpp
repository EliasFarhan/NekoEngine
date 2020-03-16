#include <gtest/gtest.h>
#include <xxhash.hpp>
#include <engine/assert.h>

namespace neko
{

TEST(Engine, Hashing)
{
    const size_t ITERATIONS = 4096;
    const size_t KEY = rand();

    xxh::hash_t<64> hash0 = xxh::xxhash<64>(&KEY, sizeof(size_t));
    xxh::hash_t<64> hash1 = hash0;
    size_t hash0sum = 0;
    size_t hash1sum = 0;
    for (size_t i = 0; i < ITERATIONS; i++)
    {
        hash0sum += hash0;
        hash1sum += hash1;
        neko_assert(hash0 == hash1, "Hash has changed!");
        hash0 = xxh::xxhash<64>(&KEY, sizeof(size_t));
        hash1 = xxh::xxhash<64>(&KEY, sizeof(size_t));
    }
    EXPECT_EQ(hash0sum, hash1sum);
}

}// !neko