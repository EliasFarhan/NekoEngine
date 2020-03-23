#include <gtest/gtest.h>
#include <engine/custom_allocator.h>
#include <utilities/map.h>
#include <vector>
#include <numeric>

namespace neko
{

using Key = char;
using Value = char;

static Key NextKey(){
    static Key k = 0;
    return k++;
}
static Value NextValue(){
    static Value v = 0;
    return v++;
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
        const size_t ALLOCATOR_HEADER_SIZE = 16; // bytes
        const size_t ALIGNMENT_PADDING = 1; // byte
        const size_t PAIRS_NUMBER = 64; // pairs
        const size_t INTERNAL_PAIR_SIZE = sizeof(std::pair<xxh::hash_t<64>, Value>); // 9 bytes used, aligned on 8 bytes => 16 bytes total.
        const size_t TOTAL_BYTES = ALLOCATOR_HEADER_SIZE + (PAIRS_NUMBER * INTERNAL_PAIR_SIZE) + ALIGNMENT_PADDING;

        MallocRAII mem(malloc(TOTAL_BYTES));
        FreeListAllocator allocator(TOTAL_BYTES, mem.data());

        try // TRY_CONSTRUCTOR
        {
            FixedMap<Key, Value, PAIRS_NUMBER> map(allocator);
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
    const size_t ALLOCATOR_HEADER_SIZE = 16; // bytes
    const size_t ALIGNMENT_PADDING = 1; // byte
    const size_t PAIRS_NUMBER = 64; // pairs
    const size_t INTERNAL_PAIR_SIZE = sizeof(std::pair<xxh::hash_t<64>, Value>); // 9 bytes used, aligned on 8 bytes => 16 bytes total.
    const size_t TOTAL_BYTES = ALLOCATOR_HEADER_SIZE + (PAIRS_NUMBER * INTERNAL_PAIR_SIZE) + ALIGNMENT_PADDING;

    MallocRAII mem(malloc(TOTAL_BYTES));
    FreeListAllocator allocator(TOTAL_BYTES, mem.data());

    FixedMap<Key, Value, PAIRS_NUMBER> map(allocator);

    try // TRY_INSERT
    {
        for (size_t i = 0; i < PAIRS_NUMBER; i++)
        {
            map.insert({NextKey(), NextValue()});
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }// !TRY_INSERT
}

TEST(Map, FixedMap_Access)
{
    const size_t ALLOCATOR_HEADER_SIZE = 16; // bytes
    const size_t ALIGNMENT_PADDING = 1; // byte
    const size_t PAIRS_NUMBER = 64; // pairs
    const size_t INTERNAL_PAIR_SIZE = sizeof(std::pair<xxh::hash_t<64>, Value>); // 9 bytes used, aligned on 8 bytes => 16 bytes total.
    const size_t TOTAL_BYTES = ALLOCATOR_HEADER_SIZE + (PAIRS_NUMBER * INTERNAL_PAIR_SIZE) + ALIGNMENT_PADDING;

    MallocRAII mem(malloc(TOTAL_BYTES));
    FreeListAllocator allocator(TOTAL_BYTES, mem.data());

    FixedMap<Key, Value, PAIRS_NUMBER> map(allocator);
    std::vector<Key> keys;
    std::vector<Value> values;
    for (size_t i = 0; i < PAIRS_NUMBER; i++)
    {
        keys.push_back(NextKey());
        values.push_back(NextValue());
        map.insert({keys.back(), values.back()});
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

    const size_t ALLOCATOR_HEADER_SIZE = 16; // bytes
    const size_t ALIGNMENT_PADDING = 1; // byte
    const size_t PAIRS_NUMBER = 64; // pairs
    const size_t INTERNAL_PAIR_SIZE = sizeof(std::pair<xxh::hash_t<64>, Value>); // 9 bytes used, aligned on 8 bytes => 16 bytes total.
    const size_t TOTAL_BYTES = ALLOCATOR_HEADER_SIZE + (PAIRS_NUMBER * INTERNAL_PAIR_SIZE) + ALIGNMENT_PADDING;

    MallocRAII mem(malloc(TOTAL_BYTES));
    FreeListAllocator allocator(TOTAL_BYTES, mem.data());

    FixedMap<Key, Value, PAIRS_NUMBER> map(allocator);
    for (size_t i = 0; i < PAIRS_NUMBER; i++)
    {
        map.insert({NextKey(), NextValue()});
    }

    try // TRY_CLEAR
    {
        map.clear();
        EXPECT_EQ(map.size(), 0);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }// !TRY_CLEAR

}

TEST(Map, FixedMap_Iterators)
{

    const size_t ALLOCATOR_HEADER_SIZE = 16; // bytes
    const size_t ALIGNMENT_PADDING = 1; // byte
    const size_t PAIRS_NUMBER = 64; // pairs
    const size_t INTERNAL_PAIR_SIZE = sizeof(std::pair<xxh::hash_t<64>, Value>); // 9 bytes used, aligned on 8 bytes => 16 bytes total.
    const size_t TOTAL_BYTES = ALLOCATOR_HEADER_SIZE + (PAIRS_NUMBER * INTERNAL_PAIR_SIZE) + ALIGNMENT_PADDING;
    using InternalPair = std::pair<xxh::hash_t<64>, Value>;

    MallocRAII mem(malloc(TOTAL_BYTES));
    FreeListAllocator allocator(TOTAL_BYTES, mem.data());

    FixedMap<Key, Value, PAIRS_NUMBER> map(allocator);
    std::vector<Key> keys;
    std::vector<Value> values;

    Key k = 0;
    for (int i = 0; i < PAIRS_NUMBER; i++)
    {
        keys.push_back(NextKey());
        values.push_back(NextValue());
        map.insert({k++,0});
    }

    try // TRY_FOREACH
    {
        int i = 0;
        std::for_each(map.begin(), map.end(), [&i, &keys, &values] (InternalPair& p) { p = InternalPair{keys[i], values[i++]};} );

        i = 0;
        std::for_each(map.begin(), map.end(), [&i](const InternalPair p){ i += p.second; });
        const int expectedSum = std::accumulate(values.begin(), values.end(), 0);

        EXPECT_EQ(expectedSum, i);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }// !TRY_FOREACH

}

}// !neko