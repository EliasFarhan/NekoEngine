//
// Created by Stephen Grosjean 24/03/2020
//

#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include "engine/custom_allocator.h"
#include "custom_container/string.h"

namespace neko
{
TEST(Engine, TestString)
{
    size_t memorySize = 1024;
    void* data = malloc(memorySize);
    FreeListAllocator allocator = FreeListAllocator(memorySize, data);
    {
        NekoString str1 = NekoString(allocator, "A");
        NekoString str2 = NekoString(allocator, "B");
        NekoString str3 = NekoString(allocator, "A");
        NekoString str4 = NekoString(allocator, "D");

        EXPECT_TRUE(str1 == "A");
        EXPECT_TRUE(str1 != "Z");
        EXPECT_TRUE(str1 == str3);
        EXPECT_TRUE(str1 != str4);

        str1 += str2;
        NekoString str5 = NekoString(allocator, "Q");
        str5 += str3;
        str5 += str2;

        EXPECT_TRUE(str1 == "AB");
        EXPECT_TRUE(str5 == "QAB");
    }
    free(data);
}
}
