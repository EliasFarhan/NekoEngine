#include <gtest/gtest.h>
#include <engine/jobsystem.h>
#include <iostream>
#include <easy/profiler.h>

namespace neko
{

void DoNothing(){
    EASY_BLOCK("JOBSYSTEM_DO_NOTHING");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Task is done!\n";
}

TEST(Engine, TestJobSystem)
{
    EASY_PROFILER_ENABLE;

    EASY_BLOCK("JOBSYSTEM_MAIN_THREAD");
    {
        const size_t TASKS_COUNT = 8;

        JobSystem system;
        for (size_t i = 0; i < TASKS_COUNT; ++i)
        {
            system.KickJob(DoNothing);
        }

        // Simulate some work on the main thread.
        std::this_thread::sleep_for(std::chrono::seconds(4));

        std::cout << "Main thread is done!\n";
    }
    EASY_END_BLOCK;

    profiler::dumpBlocksToFile("JobSystem.prof");
}

}