#pragma once

#include <vector>
#include <numeric>
#include <gtest/gtest.h>

#include <engine/custom_allocator.h>
#include <custom_container/map.h>

namespace neko
{

const size_t HASH_SIZE = 32; // bits
using Key = char;
using Value = char;
using Hash = xxh::hash_t<HASH_SIZE>;
using AllocatorType = LinearAllocator;
using InternalPair = std::pair<xxh::hash_t<HASH_SIZE>, Value>;
const size_t ALLOCATOR_HEADER_SIZE = 0; // bytes
const size_t EXTRA_PADDING = 1; // byte
const size_t PAIRS_NUMBER = 64; // pairs
const size_t PAIR_SIZE = sizeof(InternalPair);
const size_t TOTAL_BYTES = ALLOCATOR_HEADER_SIZE + (PAIRS_NUMBER * PAIR_SIZE) + EXTRA_PADDING;

static Key NextKey()
{
    static Key k = 0;
    return k++;
}

static Value NextValue()
{
    static Value v = 0;
    return v++;
}

static Key RandomNewKey()
{
    static std::vector<Key> existing;

    Key k = 0;
    do
    {
        k = std::abs((Key) rand());
    } while (std::count(existing.begin(), existing.end(), k) > 0);

    existing.push_back(k);
    return k;
}

class MallocRAII
{
public:
    explicit MallocRAII(void* mem) : mem_(mem)
    {}

    ~MallocRAII()
    {
        free(mem_);
    }

    void* data()
    {
        return mem_;
    }

private:
    void* mem_ = nullptr;
};

TEST(Map, FixedMap_InstanciationAndDestruction)
{
    try // TRY_DESTRUCTOR
    {
        MallocRAII mem(malloc(TOTAL_BYTES));
        AllocatorType allocator(TOTAL_BYTES, mem.data());

        try // TRY_CONSTRUCTOR
        {
            FixedMap <Key, Value, PAIRS_NUMBER> map(allocator);
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }// !TRY_CONSTRUCTOR
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }// !TRY_DESTRUCTOR
}

TEST(Map, FixedMap_Insertion)
{
    MallocRAII mem(malloc(TOTAL_BYTES));
    AllocatorType allocator(TOTAL_BYTES, mem.data());

    FixedMap <Key, Value, PAIRS_NUMBER> map(allocator);

    try // TRY_INSERT
    {
        for (size_t i = 0; i < PAIRS_NUMBER; i++)
        {
            map.Insert({NextKey(), NextValue()});
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }// !TRY_INSERT
}

TEST(Map, FixedMap_Access)
{
    MallocRAII mem(malloc(TOTAL_BYTES));
    AllocatorType allocator(TOTAL_BYTES, mem.data());

    FixedMap <Key, Value, PAIRS_NUMBER> map(allocator);
    std::vector<Key> keys;
    std::vector<Value> values;
    for (size_t i = 0; i < PAIRS_NUMBER; i++)
    {
        keys.push_back(NextKey());
        values.push_back(NextValue());
        map.Insert({keys.back(), values.back()});
    }

    try // TRY_ACCESS
    {
        for (size_t i = 0; i < PAIRS_NUMBER; i++)
        {
            EXPECT_EQ(values[i], map[keys[i]]);
            const Value nextValue = NextValue();
            map[keys[i]] = nextValue;
            EXPECT_EQ(nextValue, map[keys[i]]);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }// !TRY_ACCESS
}

TEST(Map, FixedMap_Clear)
{
    MallocRAII mem(malloc(TOTAL_BYTES));
    AllocatorType allocator(TOTAL_BYTES, mem.data());

    FixedMap <Key, Value, PAIRS_NUMBER> map(allocator);
    for (size_t i = 0; i < PAIRS_NUMBER; i++)
    {
        map.Insert({NextKey(), NextValue()});
    }

    try // TRY_CLEAR
    {
        map.Clear();
        EXPECT_EQ(map.Size(), 0);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }// !TRY_CLEAR

}

TEST(Map, FixedMap_Iterators)
{
    MallocRAII mem(malloc(TOTAL_BYTES));
    AllocatorType allocator(TOTAL_BYTES, mem.data());

    FixedMap <Key, Value, PAIRS_NUMBER> map(allocator);
    std::vector<Key> keys;
    std::vector<Value> values;

    Key k = 0;
    for (int i = 0; i < PAIRS_NUMBER; i++)
    {
        keys.push_back(NextKey());
        values.push_back(NextValue());
        map.Insert({k++, 0});
    }

    try // TRY_FOREACH
    {
        int i = 0;
        std::for_each(map.Begin(), map.End(),
                      [&i, &keys, &values](InternalPair& p) { p = InternalPair{keys[i], values[i++]}; });

        i = 0;
        std::for_each(map.Begin(), map.End(), [&i](const InternalPair p) { i += p.second; });
        const int expectedSum = std::accumulate(values.begin(), values.end(), 0);

        EXPECT_EQ(expectedSum, i);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }// !TRY_FOREACH

}

#undef HASH_SIZE

}// !neko
