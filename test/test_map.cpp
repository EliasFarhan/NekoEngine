#include <gtest/gtest.h>
#include <engine/custom_allocator.h>
#include <utilities/map.h>

namespace neko
{

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
        using Key = char;
        using Value = char;
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
    using Key = char;
    using Value = char;
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
            map.insert({'a', 'b'});
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }// !TRY_INSERT
}

TEST(Map, FixedMap_Access)
{
    using Key = char;
    using Value = char;
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
        map.insert({'a', 'b'});
    }

    try // TRY_ACCESS
    {
        for (size_t i = 0; i < PAIRS_NUMBER; i++)
        {
            // const char value = map[];
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }// !TRY_ACCESS
}

TEST(Map, FixedMap_Clear)
{

}

TEST(Map, FixedMap_Iterators)
{

}

}// !neko