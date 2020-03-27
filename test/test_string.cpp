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
    neko::FreeListAllocator allocator = neko::FreeListAllocator(memorySize, data);

    String str1 = String(allocator, "A");
    String str2 = String(allocator, "B");
    String str3 = String(allocator, "A");
    String str4 = String(allocator, "D");


	EXPECT_TRUE(str1 == "A");
    EXPECT_TRUE(str1 != "Z");
    EXPECT_TRUE(str1 == str3);
	EXPECT_TRUE(str1 != str4);

    str1 += str2;
    String str5 = String(allocator, "TEST");
    str5 = "";
    str5 += str3;
    str5 += str2;
	
    EXPECT_TRUE(str1 == "AB");
    EXPECT_TRUE(str5 == str1);

   // free(data);
}
}
