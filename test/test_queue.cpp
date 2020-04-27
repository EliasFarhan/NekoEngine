#include <gtest/gtest.h>
#include <iostream>
#include <utilities/custom_queue.h>

namespace neko
{
TEST(Engine, TestQueue)
{
    neko::DynamicQueue<int> QueueTest3;
    for (int i = 0; i < 10; i++) {
        QueueTest3.PushBack(i);
        EXPECT_TRUE(QueueTest3[i] == i);
        EXPECT_FALSE(QueueTest3[i] != i);
    }
}
}
