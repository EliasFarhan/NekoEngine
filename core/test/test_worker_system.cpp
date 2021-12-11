#include <gtest/gtest.h>
#include "engine/worker_system.h"

TEST(WorkerSystem, TestTask)
{
    bool value = false;
    neko::Task task{ [&value] {value = true; } };
    EXPECT_FALSE(task.HasStarted());
    EXPECT_FALSE(task.IsDone());
    EXPECT_TRUE(task.CheckDependenciesStarted());
    EXPECT_FALSE(value);
    task.Execute();
    EXPECT_TRUE(task.IsDone());
    EXPECT_TRUE(task.HasStarted());
    EXPECT_TRUE(task.CheckDependenciesStarted());
    EXPECT_TRUE(value);
    task.Reset();
    EXPECT_FALSE(task.HasStarted());
    EXPECT_FALSE(task.IsDone());
    EXPECT_TRUE(task.CheckDependenciesStarted());
    
}

TEST(WorkerSystem, TestSimpleDependency)
{
    bool value = false;
    const auto task1 = std::make_shared<neko::Task>([&value] {value = true; } );
    const auto task2 = std::make_shared<neko::Task>([&value] {value = false; } );
    EXPECT_FALSE(task2->HasStarted());
    EXPECT_FALSE(task2->IsDone());
    EXPECT_TRUE(task2->CheckDependenciesStarted());

    EXPECT_TRUE(task2->AddDependency(task1));
    //Cannot add twice the same dependency
    EXPECT_FALSE(task2->AddDependency(task1));
    //Cannot add ourselves as dependency
    EXPECT_FALSE(task2->AddDependency(task2));
    EXPECT_FALSE(task1->AddDependency(task1));
    //Cannot add a cyclic dependency
    EXPECT_FALSE(task1->AddDependency(task2));

    EXPECT_FALSE(task2->CheckDependenciesStarted());
    EXPECT_FALSE(value);
    task1->Execute();
    EXPECT_TRUE(task2->CheckDependenciesStarted());
    EXPECT_TRUE(value);
    task2->Execute();
    EXPECT_TRUE(task2->IsDone());
    EXPECT_FALSE(value);
}

TEST(WorkerSystem, TestWorkerQueue)
{
    bool value = false;
    const auto task1 = std::make_shared<neko::Task>([&value] {value = true; });
    const auto task2 = std::make_shared<neko::Task>([&value] {value = false; });
    neko::WorkerQueue workerQueue;
    EXPECT_FALSE(task2->HasStarted());
    EXPECT_FALSE(task2->IsDone());
    EXPECT_TRUE(task2->CheckDependenciesStarted());

    EXPECT_TRUE(task2->AddDependency(task1));
    //We did not put the task in the queue yet
    EXPECT_FALSE(workerQueue.Contains(task1));
    EXPECT_FALSE(workerQueue.Contains(task2));
    auto nullTask = workerQueue.PopNextTask();
    EXPECT_EQ(nullTask, nullptr);
    EXPECT_TRUE(workerQueue.AddTask(task2));
    EXPECT_TRUE(workerQueue.AddTask(task1));
    //We cannot add twice the same job
    EXPECT_FALSE(workerQueue.AddTask(task2));
    EXPECT_FALSE(workerQueue.AddTask(task1));
    //The tasks are now in the queue
    EXPECT_TRUE(workerQueue.Contains(task1));
    EXPECT_TRUE(workerQueue.Contains(task2));

    auto popTask1 = workerQueue.PopNextTask();
    EXPECT_EQ(popTask1, task2);
    auto popTask2 = workerQueue.PopNextTask();
    EXPECT_EQ(popTask2, task1);
    nullTask = workerQueue.PopNextTask();
    EXPECT_EQ(nullTask, nullptr);
}

TEST(WorkerSystem, TestWorkerThread)
{
    bool value = false;
    const auto task1 = std::make_shared<neko::Task>([&value] {value = true; });
    const auto task2 = std::make_shared<neko::Task>([&value] {value = false; });
    task2->AddDependency(task1);
    neko::WorkerQueue workerQueue;
    neko::WorkerThread workerThread(workerQueue);
    workerThread.Start();
    EXPECT_TRUE(workerQueue.AddTask(task2));
    EXPECT_TRUE(workerQueue.AddTask(task1));

    task2->Join();
    EXPECT_TRUE(task1->IsDone());
    EXPECT_TRUE(task2->IsDone());
    EXPECT_FALSE(value);
    workerQueue.Destroy();
    workerThread.Destroy();
}

TEST(WorkerSystem, TestTwoWorkerQueue)
{
    bool value = false;
    const auto task1 = std::make_shared<neko::Task>([&value] {value = true; });
    const auto task2 = std::make_shared<neko::Task>([&value] {value = false; });
    task2->AddDependency(task1);
    neko::WorkerQueue workerQueue1;
    neko::WorkerQueue workerQueue2;
    neko::WorkerThread workerThread1(workerQueue1);
    neko::WorkerThread workerThread2(workerQueue2);
    workerThread1.Start();
    workerThread2.Start();
    EXPECT_TRUE(workerQueue1.AddTask(task2));
    EXPECT_TRUE(workerQueue2.AddTask(task1));

    task2->Join();
    EXPECT_TRUE(task1->IsDone());
    EXPECT_TRUE(task2->IsDone());
    EXPECT_FALSE(value);
    workerQueue1.Destroy();
    workerQueue2.Destroy();
    workerThread1.Destroy();
    workerThread2.Destroy();
}
TEST(WorkerSystem, TestEmptyWorkerManager)
{
    neko::WorkerManager workerManager;
    core::pb::WorkerManager workerManagerPb;

    
    auto* mainQueue = workerManagerPb.mutable_main_queue();
    mainQueue->set_name("main");
    auto* queue = workerManagerPb.add_other_queues();
    queue->set_name("other");
    auto* thread1 = workerManagerPb.add_other_threads();
    thread1->set_allocated_queue(queue);
    auto* thread2 = workerManagerPb.add_other_threads();
    thread2->set_allocated_queue(queue);
    workerManager.Init(workerManagerPb);
    
    workerManager.ExecuteMainThread();
    workerManager.Destroy();
}
TEST(WorkerSystem, TestWorkerManager)
{
    neko::WorkerManager workerManager;
    core::pb::WorkerManager workerManagerPb;
    constexpr std::string_view queueName = "other";
    auto* queue = workerManagerPb.add_other_queues();
    queue->set_name(queueName.data());
    auto* thread1 = workerManagerPb.add_other_threads();
    thread1->set_allocated_queue(queue);
    auto* thread2 = workerManagerPb.add_other_threads();
    thread2->set_allocated_queue(queue);
    workerManager.Init(workerManagerPb);

    bool value = false;
    const auto task1 = std::make_shared<neko::Task>([&value] {value = true; });
    const auto task2 = std::make_shared<neko::Task>([&value] {value = false; });
    task2->AddDependency(task1);

    workerManager.AddTask(task2, queueName);
    workerManager.AddTask(task1, queueName);
    task2->Join();
    EXPECT_TRUE(task1->IsDone());
    EXPECT_TRUE(task2->IsDone());
    EXPECT_FALSE(value);
    workerManager.Destroy();
}