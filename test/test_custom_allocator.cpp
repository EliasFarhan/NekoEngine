//
// Created by efarhan on 01.03.20.
//

#include "engine/custom_allocator.h"
#include "gtest/gtest.h"

TEST(Engine, TestCustomAllocator)
{
    int value = 3;
    int* ptr = &value;

    EXPECT_EQ(neko::Allocator::CalculateAlignForwardAdjustment(ptr, alignof(int)), 0);
    EXPECT_EQ(neko::Allocator::CalculateAlignForwardAdjustment((int*)((std::uint64_t)ptr+1), alignof(int)), 3);
    EXPECT_EQ(neko::Allocator::CalculateAlignForwardAdjustment((int*)((std::uint64_t)ptr+2), alignof(int)), 2);
    EXPECT_EQ(neko::Allocator::CalculateAlignForwardAdjustment((int*)((std::uint64_t)ptr+3), alignof(int)), 1);

    std::uint16_t header = 365;

    EXPECT_EQ(neko::Allocator::CalculateAlignForwardAdjustmentWithHeader(ptr, alignof(int), sizeof(header)), 4);

}