#include <numeric>

#include "gtest/gtest.h"
#include "ntl/array.h"
#include "ntl/map.h"

TEST(Engine, TestFixedArray)
{
    const size_t length = 100;
    const size_t allocationSize = length + 2*sizeof(ptrdiff_t)-1;
    void* data = std::calloc(allocationSize, sizeof(int));
    neko::StackAllocator allocator(allocationSize * sizeof(int), data);
    {
        neko::FixedArray<int, length> array(allocator);
        std::iota(array.begin(), array.end(), 0);
        EXPECT_EQ(length - 1, array[length - 1]);
        EXPECT_NE(allocator.GetUsedMemory(), 0);
    }
    EXPECT_EQ(allocator.GetUsedMemory(), 0);
}

TEST(Engine, TestFixedArrayList)
{
    const size_t length = 100;
    const size_t allocationSize = 3*(length + 2 * sizeof(ptrdiff_t) - 1);
    void* data = std::calloc(allocationSize, sizeof(int));
    {
        neko::StackAllocator allocator(allocationSize * sizeof(int), data);
        {
            neko::FixedArrayList<int, length> array(allocator);
            const int value = 3;
            array.insert(array.begin(), value);
            EXPECT_EQ(array.front(), value);
            EXPECT_EQ(array.size(), 1);
            EXPECT_EQ(array.capacity(), length);
            array.erase(array.cbegin());
            EXPECT_EQ(array.size(), 0);
            for(size_t i = 0; i < array.capacity(); i++)
            {
                array.push_back(static_cast<int>(i));
            }
            EXPECT_EQ(array.size(), length);
            array.erase(array.cbegin() + length / 2, array.cend());
            EXPECT_EQ(array.size(), length / 2);
            EXPECT_EQ(array.back(), length/2-1);
            for(size_t i = 0; i < array.size(); i++)
            {
                EXPECT_EQ(array[i], i);
            }
            neko::FixedArrayList<int, length> array2(array);
            EXPECT_EQ(array.size(), array2.size());
            array.clear();
            EXPECT_EQ(array.size(), 0);
        }
        EXPECT_EQ(allocator.GetUsedMemory(), 0);
    }
    free(data);
}

TEST(Engine, TestArrayList)
{
    const size_t length = 100;
    const size_t allocationSize = length * 4;
    void* data = std::calloc(allocationSize, sizeof(int));
    {
        neko::FreeListAllocator allocator(
            sizeof(int) * allocationSize, data);
        neko::StandardLibraryAllocator<int> stdAllocator(allocator);
        {
            neko::ArrayList<int> numbers(stdAllocator);
            for (size_t i = 0; i < length; i++)
            {
                numbers.push_back(static_cast<int>(i));
            }
            EXPECT_EQ(0, numbers.front());
            EXPECT_EQ(length - 1, numbers.back());


            std::cout << "[TEST] Allocator Size: " << allocator.GetSize() << " Used Memory: " << allocator.GetUsedMemory() << '\n';
            EXPECT_NE(allocator.GetUsedMemory(), 0);
        }

        EXPECT_EQ(allocator.GetUsedMemory(), 0);
        std::cout << "[TEST] Allocator Size: " << allocator.GetSize() << " Used Memory: " << allocator.GetUsedMemory() << '\n';
    }
    
    free(data);
}

TEST(Engine, TestFixedMap)
{
    const size_t length = 100;
    const size_t allocationSize = length * 4;
    void* data = std::calloc(allocationSize, sizeof(int));
    {
        neko::FreeListAllocator allocator(sizeof(int) * allocationSize, data);
        {
            neko::FixedMap<int, int, length> map(allocator);
            map[43] = 12;
            map[8] = 16;
            EXPECT_EQ(map[43], 12);
            EXPECT_EQ(map[8], 16);
        }
    }

}

TEST(Engine, TestSortedMap)
{
    const size_t length = 100;
    const size_t allocationSize = length * 4;
    void* data = std::calloc(allocationSize, sizeof(int));
    {
        neko::FreeListAllocator allocator(
            sizeof(int) * allocationSize, data);
        const neko::StandardLibraryAllocator<std::pair<const int, int>> stdAllocator(allocator);
        {
            neko::SortedMap<int, int> map(stdAllocator);
            map[32] = 15;
            EXPECT_EQ(map[32], 15);
            map[42] = 18;
            EXPECT_EQ(map[42], 18);

            std::cout << "[TEST] Allocator Size: " << allocator.GetSize() << " Used Memory: " << allocator.GetUsedMemory() << '\n';
        }

        EXPECT_EQ(allocator.GetUsedMemory(), 0);
        std::cout << "[TEST] Allocator Size: " << allocator.GetSize() << " Used Memory: " << allocator.GetUsedMemory() << '\n';
    }

    free(data);
}