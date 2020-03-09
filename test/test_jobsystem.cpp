#include <gtest/gtest.h>
#include <engine/jobsystem.h>
//#include <easy/profiler.h>

namespace neko
{

void IncrementMem(std::atomic<unsigned int>& mem)
{
    const unsigned int TASK_WORK_TIME = 1;

#ifdef USING_EASY_PROFILER
    EASY_BLOCK("JOBSYSTEM_DO_NOTHING");
#endif
    std::this_thread::sleep_for(std::chrono::seconds(TASK_WORK_TIME));
    mem++;
}

TEST(Engine, TestJobSystem)
{
    const unsigned int TASKS_COUNT = 16;
    std::atomic<unsigned int> tasksDone = 0;

    {// JobSystem
#ifdef USING_EASY_PROFILER
        EASY_PROFILER_ENABLE;
        EASY_BLOCK("JOBSYSTEM_MAIN_THREAD"){
#endif
            JobSystem system;
            for (size_t i = 0; i < TASKS_COUNT; ++i)
            {
                system.KickJob(std::function<void()>{[&tasksDone] { IncrementMem(tasksDone); }});
            }

#ifdef USING_EASY_PROFILER
        } EASY_END_BLOCK;
#endif
    }// !JobSystem

#ifdef USING_EASY_PROFILER
    profiler::dumpBlocksToFile("JobSystem.prof");
#endif

    // JobSystem must make main thread wait until all tasks are done before self-destructing.
    EXPECT_EQ(TASKS_COUNT, tasksDone);
}

}