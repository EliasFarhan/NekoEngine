#include <gtest/gtest.h>
#include <iostream>
#include <utilities/custom_queue.h>

namespace neko
{
TEST(Engine, TestQueue)
{
    neko::DynamicQueue<int> QueueTest;
    for (int i = 0; i < 10; i++) {
        QueueTest.PushBack(i);
        EXPECT_TRUE(QueueTest.Back() == i);
        EXPECT_FALSE(QueueTest.Back() != i);
    }
    for (int i = 0; i < 10; i++) {
        
        EXPECT_TRUE(QueueTest.Front() == i);
        EXPECT_FALSE(QueueTest.Front() != i);
        QueueTest.PopFront();
    }
	
}
TEST(Engine, TestBasicQueue)
{
 
    neko::DynamicQueueBasic<int> QueueTest;
    for (int i = 0; i < 10; i++) {
        QueueTest.PushBack(i);
        EXPECT_TRUE(QueueTest.Back() == i);
        EXPECT_FALSE(QueueTest.Back() != i);
    }
    for (int i = 0; i < 10; i++) {

        EXPECT_TRUE(QueueTest.Front() == i);
        EXPECT_FALSE(QueueTest.Front() != i);
        QueueTest.PopFront();
    }
}
}
