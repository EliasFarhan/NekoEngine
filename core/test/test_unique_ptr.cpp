//
// Created by efarhan on 3/24/22.
//

#include "container/unique_ptr.h"
#include <gtest/gtest.h>

TEST(UniquePtr, DefaultConstructor)
{
    neko::tl::UniquePtr<int> emptyPtr;
}

TEST(UniquePtr, Constructor)
{
    neko::DumbAllocator allocator;
    int* newInt = static_cast<int*>(allocator.Allocate(sizeof(int), alignof(int)));
    *newInt = 3;
    neko::tl::UniquePtr<int> ptr(newInt, allocator);
    EXPECT_EQ(ptr.Get(), newInt);
    EXPECT_EQ(*ptr, *newInt);

    auto moveAssignPtr = std::move(ptr);
    EXPECT_EQ(moveAssignPtr.Get(), newInt);
    EXPECT_EQ(ptr.Get(), nullptr);
    neko::tl::UniquePtr<int> moveConstructPtr(std::move(moveAssignPtr));
    EXPECT_EQ(moveConstructPtr.Get(), newInt);
    EXPECT_EQ(moveAssignPtr.Get(), nullptr);

    int* secondInt = static_cast<int*>(allocator.Allocate(sizeof(int), alignof(int)));
    *secondInt = 4;
    neko::tl::UniquePtr<int> secondPtr (secondInt, allocator);
    EXPECT_EQ(secondInt, secondPtr.Get());
    EXPECT_EQ(*secondInt, *secondPtr);
    ptr.Swap(secondPtr);
    EXPECT_EQ(secondInt, ptr.Get());
    EXPECT_EQ(*secondInt, *ptr);
}

TEST(UniquePtr, UniquePtrFactory)
{
    neko::DumbAllocator allocator;
    neko::tl::UniquePtrFactory<int> intFactory(allocator);

    auto newIntPtr = intFactory.MakeUnique(4);
    EXPECT_EQ(*newIntPtr, 4);
    auto secondIntPtr = intFactory.MakeUnique(5);
    EXPECT_EQ(*secondIntPtr, 5);
}