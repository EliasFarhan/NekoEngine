#include <gtest/gtest.h>
#include <iostream>
#include <utilities/custom_queue.h>

namespace neko
{
TEST(Engine, TestQueue)
{
    neko::SmallQueue<int  , 10> QueueTest;
 
    for (int i = 0; i < 10; i++) {

        QueueTest.PushBack(i);
    
        EXPECT_TRUE(QueueTest[i] == i);
    }
    neko::DynamicQueueBasic<int> QueueTest2;
    for (int i = 0; i < 10; i++) {

        QueueTest2.PushBack(i);

        EXPECT_TRUE(QueueTest2[i] == i);
    }
    neko::DynamicQueue<int> QueueTest3;
    for (int i = 0; i < 10; i++) {

        QueueTest3.PushBack( i);

        EXPECT_TRUE(QueueTest3[ i] == i );
    }
}
}
